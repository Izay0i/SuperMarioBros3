#include "../../headers/npc/BoomerBro.h"

LPCWSTR BoomerBro::texturePath = nullptr;
LPDIRECT3DTEXTURE9 BoomerBro::texture = nullptr;
D3DCOLOR BoomerBro::colorKey = D3DCOLOR_XRGB(0, 0, 0);

BoomerBro::BoomerBro() {
	//0 - die
	//1 - walk
	hitPoints = 1;
	normal = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	scale = D3DXVECTOR2(-1.0f, 1.0f);
}

RECTF BoomerBro::GetBoundingBox(int id) const {
	RECTF bound;
	
	if (hitPoints > 0) {
		bound.left = position.x;
		bound.top = position.y;
		bound.right = position.x + hitBox.GetWidth(id);
		bound.bottom = position.y + hitBox.GetHeight(id);
	}

	return bound;
}

void BoomerBro::LoadTexture() {
	if (!texture) {
		HRESULT hResult;
		D3DXIMAGE_INFO imageInfo;

		hResult = D3DXGetImageInfoFromFile(texturePath, &imageInfo);
		if (hResult != D3D_OK) {
			OutputDebugStringA("[QUESTION BLOCK] Failed to get image info\n");
			return;
		}

		hResult = D3DXCreateTextureFromFileEx(
			directDevice,
			texturePath,
			imageInfo.Width,
			imageInfo.Height,
			1,
			D3DUSAGE_DYNAMIC,
			D3DFMT_UNKNOWN,
			D3DPOOL_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			colorKey,
			&imageInfo,
			nullptr,
			&texture
		);

		if (hResult != D3D_OK) {
			OutputDebugStringA("Failed to create entity sprite from file\n");
			return;
		}
	}
}

void BoomerBro::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void BoomerBro::ParseHitboxes(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 4) {
		return;
	}

	float left = std::stof(tokens.at(0));
	float top = std::stof(tokens.at(1));
	float right = std::stof(tokens.at(2));
	float bottom = std::stof(tokens.at(3));

	RECTF hitbox;
	hitbox.left = left;
	hitbox.top = top;
	hitbox.right = right;
	hitbox.bottom = bottom;

	this->hitBox.AddHitBox(hitbox);
}

void BoomerBro::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::vector<std::string> extraData) {
	std::ifstream readFile;
	readFile.open(dataPath, std::ios::in);

	if (!readFile.is_open()) {
		OutputDebugStringA("Failed to read data\n");
		return;
	}

	if (extraData.size() > 0) {
		this->extraData = extraData;
	}

	this->texturePath = Util::ToLPCWSTR(texturePath);
	this->colorKey = colorKey;

	LoadTexture();

	DataSection section = DataSection::DATA_SECTION_UNKNOWN;

	char str[MAX_FILE_LINE];
	while (readFile.getline(str, MAX_FILE_LINE)) {
		std::string line(str);

		if (line.empty() || line.front() == '#') {
			continue;
		}

		if (line == "[SPRITES]") {
			section = DataSection::DATA_SECTION_SPRITES;
			continue;
		}

		if (line == "[HITBOXES]") {
			section = DataSection::DATA_SECTION_HITBOXES;
			continue;
		}

		switch (section) {
		case DataSection::DATA_SECTION_SPRITES:
			ParseSprites(line);
			break;
		case DataSection::DATA_SECTION_HITBOXES:
			ParseHitboxes(line);
			break;
		}
	}

	readFile.close();
}

void BoomerBro::HandleStates() {
	switch (hitPoints) {
		case 0:
			currentState = BroState::DIE;
			break;
		case 1:
			currentState = BroState::WALK;
			break;
	}

	switch (currentState) {
		case BroState::DIE:
			velocity.x = 0;
			scale.y = -1.0f;
			break;
		case BroState::WALK:
			velocity.x = -runSpeed * normal.x;
			break;
	}
}

Boomerang* BoomerBro::SpawnBoomerang() {
	Boomerang* boomerang = new Boomerang();
	boomerang->SetObjectID(97);
	boomerang->ParseData(extraData.at(0), extraData.at(1), colorKey);
	boomerang->SetNormal(D3DXVECTOR3(normal.x, 0, 0));
	boomerang->SetPosition(D3DXVECTOR3(position.x + 5, position.y + 10, 0));
	return boomerang;
}

void BoomerBro::TakeDamage() {
	hitPoints = 0;
	StartRemoveTimer();

	tookDamage = true;

	score *= multiplier;
	multiplier *= 2;
	StartResetScoreTimer();
}

void BoomerBro::Update(DWORD delta, std::vector<GameObject*>* objects) {
	HandleStates();

	/*char debug[100];
	sprintf_s(debug, "Velx: %f\n", velocity.x);
	OutputDebugStringA(debug);*/

	if (position.x <= originalPos.x - MIN_X_OFFSET) {
		normal.x = -1.0f;
	}
	else if (position.x >= originalPos.x + MAX_X_OFFSET) {
		normal.x = 1.0f;
	}

	if (!IsAttacking()) {
		SceneManager::GetInstance()->GetCurrentScene()->AddObjectToScene(SpawnBoomerang());
		StartAttackTimer();
	}

	if (attackStart != 0 && GetTickCount64() - attackStart > attackTime) {
		attackStart = 0;
	}

	if (resetScoreStart != 0 && GetTickCount64() - resetScoreStart > resetScoreTime) {
		multiplier = 1;
		resetScoreStart = 0;
	}

	if (removeStart != 0 && GetTickCount64() - removeStart > removeTime) {
		hitPoints = -1;
		removeStart = 0;
	}

	GameObject::Update(delta);
	velocity.y += gravity * delta;
	
	std::vector<LPCOLLISIONEVENT> collisionEvents, eventResults;
	collisionEvents.clear();

	if (hitPoints > 0) {
		CalcPotentialCollision(objects, collisionEvents);
	}

	if (collisionEvents.size() == 0) {
		position += distance;
	}
	else {
		D3DXVECTOR2 minTime;
		D3DXVECTOR2 offSet(0.4f, 0.4f);
		D3DXVECTOR3 normal;
		D3DXVECTOR3 relativeDistance;

		FilterCollision(collisionEvents, eventResults, minTime, normal, relativeDistance);	

		for (LPCOLLISIONEVENT result : eventResults) {
			LPCOLLISIONEVENT event = result;

			//mario's fireball
			if (dynamic_cast<Entity*>(event->object) && event->object->GetObjectID() == 99) {
				tookDamage = true;
				hitPoints = 0;
				velocity.y = -0.23f;
			}

			//ditto
			if (dynamic_cast<KoopaTroopa*>(event->object) &&
				(dynamic_cast<KoopaTroopa*>(event->object)->GetCurrentHitPoints() == 1 || isBeingHeld))
			{
				KoopaTroopa* koopa = static_cast<KoopaTroopa*>(event->object);
				koopa->SetCurrenHitPoints(0);
				koopa->SetScale(D3DXVECTOR2(1.0f, -1.0f));
				koopa->SetVelocity(D3DXVECTOR3(0, -0.23f, 0));

				tookDamage = true;
				hitPoints = 0;
				scale = D3DXVECTOR2(1.0f, -1.0f);
				velocity.y = -0.23f;
			}

			//boomerang
			if (dynamic_cast<Boomerang*>(event->object)) {
				Boomerang* boomerang = static_cast<Boomerang*>(event->object);
				boomerang->SetCurrenHitPoints(-1);
			}
		}

		if (normal.x != 0.0f) {
			velocity.x = 0.0f;
		}

		if (normal.y != 0.0f) {
			velocity.y = 0.0f;
		}

		position.x += minTime.x * distance.x + normal.x * offSet.x;
		position.y += minTime.y * distance.y + normal.y * offSet.y;
	}

	for (LPCOLLISIONEVENT event : collisionEvents) {
		delete event;
	}
}

void BoomerBro::Render() {
	switch (currentState) {
		case BroState::WALK:
			sprite.PlayAnimation("Walk", position, scale);
			
			if (attackStart != 0 && GetTickCount64() - attackStart > (attackTime * 3 / 4)) {
				sprite.PlayAnimation("Boomerang", D3DXVECTOR3(position.x - 5, position.y - 5, 0), scale);
			}
			break;
		case BroState::DIE:
			sprite.PlayAnimation("Die", position, scale);
			break;
	}

	if (resetScoreStart != 0 && GetTickCount64() - resetScoreStart > resetScoreTime / 4) {
		sprite.PlayAnimation(ScoreToString(score), D3DXVECTOR3(position.x + 1, position.y - 16, 0));
	}
}

void BoomerBro::Release() {

}
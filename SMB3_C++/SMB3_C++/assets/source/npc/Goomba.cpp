#include "../../headers/npc/Goomba.h"

LPCWSTR Goomba::texturePath = nullptr;
LPDIRECT3DTEXTURE9 Goomba::texture = nullptr;
D3DCOLOR Goomba::colorKey = D3DCOLOR_XRGB(0, 0, 0);

Goomba::Goomba() {
	//0 - dead
	//1 - walk
	hitPoints = 1;
	scale = D3DXVECTOR2(1.0f, 1.0f);
}

void Goomba::LoadTexture() {
	if (!texture) {
		HRESULT hResult;
		D3DXIMAGE_INFO imageInfo;

		hResult = D3DXGetImageInfoFromFile(texturePath, &imageInfo);
		if (hResult != D3D_OK) {
			OutputDebugStringA("[MARIO] Failed to get image info\n");
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

RECTF Goomba::GetBoundingBox(int id) const {
	RECTF bound;	

	if (hitPoints > 0) {
		bound.left = position.x;
		bound.top = position.y;
		bound.right = position.x + hitBox.GetWidth(id);
		bound.bottom = position.y + hitBox.GetHeight(id);
	}

	return bound;
}

void Goomba::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void Goomba::ParseHitboxes(std::string line) {
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

void Goomba::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::vector<std::string> extraData) {
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

void Goomba::HandleStates() {
	switch (hitPoints) {
		case 0:
			currentState = GoombaState::DIE;
			break;
		case 1:
			currentState = GoombaState::WALK;
			break;
	}

	switch (currentState) {
		case GoombaState::WALK:
			velocity.x = -runSpeed * normal.x;
			break;
		case GoombaState::DIE:
			velocity.x = 0;

			if (animName == "Die") {
				velocity.y = 0;
			}
			break;
	}
}

void Goomba::TakeDamage() {
	if (hitPoints > 0) {
		--hitPoints;
	}

	if (hitPoints == 0) {
		StartRemoveTimer();
	}
}

void Goomba::Update(DWORD delta, std::vector<GameObject*>* objects) {	
	HandleStates();

	GameObject::Update(delta);

	velocity.y += gravity * delta;

	if (removeStart != 0 && GetTickCount64() - removeStart > removeTime) {
		hitPoints = -1;
		removeStart = 0;
	}

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
		D3DXVECTOR3 normal;
		D3DXVECTOR3 relativeDistance;

		FilterCollision(collisionEvents, eventResults, minTime, normal, relativeDistance);

		position.x += minTime.x * distance.x + normal.x * 0.4f;
		position.y += minTime.y * distance.y + normal.y * 0.4f;

		if (normal.x != 0.0f) {
			velocity.x = 0.0f;
		}

		if (normal.y != 0.0f) {
			velocity.y = 0.0f;
		}

		for (LPCOLLISIONEVENT result : eventResults) {
			LPCOLLISIONEVENT event = result;

			//mario's fireball or koopa shell
			if (dynamic_cast<Entity*>(event->object) && event->object->GetObjectID() == 99 ||
				((event->object->GetObjectID() == 3 || event->object->GetObjectID() == 4) && dynamic_cast<Entity*>(event->object)->GetCurrentHitPoints() == 1)) 
			{				
				animName = "Walk";
				scale = D3DXVECTOR2(1.0f, -1.0f);
				velocity.y = -0.23f;
				hitPoints = 0;
			}

			if (dynamic_cast<Entity*>(event->object) || dynamic_cast<Tiles*>(event->object)) {
				if (event->normal.x != 0.0f) {
					this->normal.x = -event->normal.x;
				}
			}			
		}
	}

	for (LPCOLLISIONEVENT event : collisionEvents) {
		delete event;
	}
}

void Goomba::Render() {
	switch (currentState) {
		case GoombaState::WALK:
			sprite.PlayAnimation("Walk", D3DXVECTOR3(position.x, position.y - 1, 0));
			break;
		case GoombaState::DIE:
			sprite.PlayAnimation(animName, D3DXVECTOR3(position.x, position.y - 1, 0), scale);
			break;
	}
}

void Goomba::Release() {

}
#include "../../headers/npc/Parakoopa.h"

LPCWSTR Parakoopa::texturePath = nullptr;
LPDIRECT3DTEXTURE9 Parakoopa::texture = nullptr;
D3DCOLOR Parakoopa::colorKey = D3DCOLOR_XRGB(0, 0, 0);

Parakoopa::Parakoopa() {
	//0 - dead
	//1 - spin
	//2 - shell
	//3 - normal
	//4 - flying
	hitPoints = 4;
}

void Parakoopa::LoadTexture() {
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

RECTF Parakoopa::GetBoundingBox(int id) const {
	RECTF bound;
	bound.left = position.x + 1;
	bound.top = position.y;

	if (hitPoints >= 3) {
		bound.right = position.x + hitBox.GetWidth(id);
		bound.bottom = position.y + hitBox.GetHeight(id);
	}
	else {
		bound.right = position.x + hitBox.GetWidth(1);
		bound.bottom = position.y + hitBox.GetHeight(1);
	}

	return bound;
}

void Parakoopa::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void Parakoopa::ParseHitboxes(std::string line) {
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

void Parakoopa::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey) {
	std::ifstream readFile;
	readFile.open(dataPath, std::ios::in);

	if (!readFile.is_open()) {
		OutputDebugStringA("Failed to read data\n");
		return;
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

void Parakoopa::HandleStates() {
	switch (hitPoints) {
		case 0:
			currentState = ParakoopaState::DIE;
			break;
		case 1:
			currentState = ParakoopaState::SPIN;
			break;
		case 2:
			currentState = ParakoopaState::RETRACT;
			break;
		case 3:
			currentState = ParakoopaState::WALK;
			break;
		case 4:
			currentState = ParakoopaState::FLY;
			break;
	}

	switch (currentState) {
		case ParakoopaState::SPIN:
			velocity.x = -runSpeed * normal.x * 6.0f;
			break;
		case ParakoopaState::FLY:
		case ParakoopaState::WALK:
			velocity.x = -runSpeed * normal.x;
			scale = D3DXVECTOR2(1.0f, 1.0f);
			break;
		case ParakoopaState::RETRACT:
		case ParakoopaState::DIE:
			velocity = D3DXVECTOR3(0, 0, 0);
			break;
	}
}

void Parakoopa::TakeDamage() {
	if (hitPoints >= 3) {
		--hitPoints;
		StartRetract();
	}
}

void Parakoopa::Update(DWORD delta, std::vector<GameObject*>* objects) {
	HandleStates();

	GameObject::Update(delta);

	velocity.y += gravity * delta;

	if (hitPoints == 4 && isOnGround) {
		velocity.y = -jumpSpeed * delta;
		isOnGround = false;
	}

	if (retractStart != 0) {
		if (GetTickCount64() - retractStart > retractTime) {
			retractStart = 0;
			hitPoints = 3;
		}
	}

	//shell spinning
	if (hitPoints == 1) {
		StartRetract();
	}

	std::vector<LPCOLLISIONEVENT> collisionEvents, eventResults;
	collisionEvents.clear();

	if (hitPoints != 0) {
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

			if (dynamic_cast<Tiles*>(event->object) ||
				dynamic_cast<QuestionBlock*>(event->object) ||
				dynamic_cast<ShinyBrick*>(event->object))
			{
				if (event->normal.y == -1.0f) {
					isOnGround = true;
				}
			}

			if (dynamic_cast<Entity*>(event->object)) {
				Entity* entity = static_cast<Entity*>(event->object);
				if (hitPoints == 1) {
					entity->TakeDamage();

					if (dynamic_cast<ShinyBrick*>(event->object)) {
						entity->SetCurrenHitPoints(0);
					}
				}
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

void Parakoopa::Render() {
	switch (currentState) {
		case ParakoopaState::FLY:
			sprite.PlayAnimation("Fly", position, D3DXVECTOR2(normal.x, 1.0f));
			break;
		case ParakoopaState::WALK:
			sprite.PlayAnimation("Walk", position, D3DXVECTOR2(normal.x, 1.0f));
			break;
		case ParakoopaState::RETRACT:
			sprite.PlayAnimation("Retract", D3DXVECTOR3(position.x, position.y + 10, 0), scale);

			if (GetTickCount64() - retractStart > (retractTime * 3 / 4)) {
				sprite.PlayAnimation("Out", D3DXVECTOR3(position.x, position.y + 10, 0), scale);
			}
			break;
		case ParakoopaState::SPIN:
			sprite.PlayAnimation("Spin", D3DXVECTOR3(position.x, position.y - 1, 0), scale);
			break;
		case ParakoopaState::DIE:
			sprite.PlayAnimation("Retract", position, scale);
			break;
	}
}

void Parakoopa::Release() {

}
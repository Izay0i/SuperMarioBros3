#include "../../headers/npc/Fireball.h"

LPCWSTR Fireball::texturePath = nullptr;
LPDIRECT3DTEXTURE9 Fireball::texture = nullptr;
D3DCOLOR Fireball::colorKey = D3DCOLOR_XRGB(0, 0, 0);

Fireball::Fireball() {
	hitPoints = 1;
	currentState = BallState::BOUNCE;

	StartCountDownTimer();
}

void Fireball::LoadTexture() {
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

RECTF Fireball::GetBoundingBox(int id) const {
	RECTF bound;
	
	if (hitPoints > 0) {
		bound.left = position.x;
		bound.top = position.y;
		bound.right = position.x + hitBox.GetWidth(id);
		bound.bottom = position.y + hitBox.GetHeight(id);
	}

	return bound;
}

void Fireball::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void Fireball::ParseHitboxes(std::string line) {
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

void Fireball::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::vector<std::string> extraData) {
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

void Fireball::HandleStates() {
	switch (hitPoints) {
		case 0:
			currentState = BallState::EXPLODE;
			break;
		case 1:
			currentState = BallState::BOUNCE;
			break;
	}

	switch (currentState) {
		case BallState::BOUNCE:
			if (objectID == 98) {
				runSpeed = 0.05f;
			}

			velocity.x = runSpeed * normal.x;
			break;
		case BallState::EXPLODE:
			velocity = D3DXVECTOR3(0, 0, 0);
			break;
	}
}

void Fireball::TakeDamage() {
	if (hitPoints > 0) {
		--hitPoints;
	}
}

void Fireball::Update(DWORD delta, std::vector<GameObject*>* objects) {
	//objectID:
	//98 - venus
	//99 - mario

	if (hitPoints == 0 && !IsBeingRemoved()) {
		StartRemoveTimer();
	}

	HandleStates();

	GameObject::Update(delta);

	if (objectID == 98) {
		velocity.y = -travelSpeed * normal.y * delta;
	}
	else if (objectID == 99) {
		velocity.y += gravity * delta;
	}
	
	if (aliveStart != 0 && GetTickCount64() - aliveStart > aliveTime) {
		hitPoints = -1;
		aliveStart = 0;
	}

	if (removeStart != 0 && GetTickCount64() - removeStart > removeTime) {
		hitPoints = -1;
		removeStart = 0;
	}

	std::vector<LPCOLLISIONEVENT> collisionEvents, eventResults;
	collisionEvents.clear();

	if (hitPoints != 0) {
		CalcPotentialCollision(objects, collisionEvents);
	}

	if (collisionEvents.size() == 0 || objectID == 98) {
		position += distance;
	}
	else {
		D3DXVECTOR2 minTime;
		D3DXVECTOR2 offSet(0.4f, 0.4f);
		D3DXVECTOR3 normal;
		D3DXVECTOR3 relativeDistance;

		FilterCollision(collisionEvents, eventResults, minTime, normal, relativeDistance);	

		position.x += minTime.x * distance.x + normal.x * offSet.x;
		position.y += minTime.y * distance.y + normal.y * offSet.y;

		if (normal.x != 0.0f) {
			velocity.x = 0.0f;
		}

		if (normal.y != 0.0f) {
			if (objectID == 99) {
				velocity.y = -jumpSpeed;
			}
		}

		for (LPCOLLISIONEVENT result : eventResults) {
			LPCOLLISIONEVENT event = result;			

			if (objectID == 99) {
				if (dynamic_cast<Tiles*>(event->object)) {
					if (event->normal.x != 0.0f) {
						TakeDamage();
					}
				}
				else if (dynamic_cast<Entity*>(event->object)) {
					Entity* entity = static_cast<Entity*>(event->object);
					entity->TakeDamage();
					TakeDamage();
				}
			}

			//ignore top and bottom of question block and shiny brick
			if (event->object->GetObjectID() == 102 || event->object->GetObjectID() == 103) {
				if (event->normal.y != 0.0f) {
					continue;
				}
			}

			//ignore coins
			//ignore one-way platforms
			if (event->object->GetObjectID() == 101 || event->object->GetObjectID() == 205) {
				continue;
			}	
		}
	}

	for (LPCOLLISIONEVENT event : collisionEvents) {
		delete event;
	}
}

void Fireball::Render() {
	switch (currentState) {
		case BallState::BOUNCE:
			sprite.PlayAnimation("Bounce", position, D3DXVECTOR2(normal.x, 1.0f));
			break;
		case BallState::EXPLODE:
			sprite.PlayAnimation("Explode", position);
			break;
	}
}

void Fireball::Release() {
	OutputDebugStringA("Fireball released\n");
}
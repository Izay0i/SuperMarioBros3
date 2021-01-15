#include "../../headers/npc/KoopaTroopa.h"

LPCWSTR KoopaTroopa::texturePath = nullptr;
LPDIRECT3DTEXTURE9 KoopaTroopa::texture = nullptr;
D3DCOLOR KoopaTroopa::colorKey = D3DCOLOR_XRGB(0, 0, 0);

KoopaTroopa::KoopaTroopa() {
	//0 - dead
	//1 - spin
	//2 - shell
	//3 - normal
	hitPoints = 3;
}

void KoopaTroopa::LoadTexture() {
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

RECTF KoopaTroopa::GetBoundingBox(int id) const {
	RECTF bound;
	
	if (hitPoints > 0) {
		bound.left = position.x;
		bound.top = position.y;

		if (hitPoints >= 3) {
			bound.right = position.x + hitBox.GetWidth(id);
			bound.bottom = position.y + hitBox.GetHeight(id);
		}
		else {
			bound.right = position.x + hitBox.GetWidth(1);
			bound.bottom = position.y + hitBox.GetHeight(1);
		}
	}

	return bound;
}

void KoopaTroopa::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void KoopaTroopa::ParseHitboxes(std::string line) {
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

void KoopaTroopa::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::vector<std::string> extraData) {
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

void KoopaTroopa::HandleStates() {
	switch (hitPoints) {
		case 0:
			currentState = KoopaState::DIE;
			break;
		case 1:
			currentState = KoopaState::SPIN;
			break;
		case 2:
			currentState = KoopaState::RETRACT;
			break;
		case 3:
			currentState = KoopaState::WALK;
			break;
	}

	switch (currentState) {
		case KoopaState::SPIN:
			velocity.x = -runSpeed * normal.x * 6.0f;
			break;
		case KoopaState::WALK:
			velocity.x = -runSpeed * normal.x;
			scale = D3DXVECTOR2(1.0f, 1.0f);
			break;
		case KoopaState::RETRACT:
			velocity = D3DXVECTOR3(0, 0, 0);
			break;
		case KoopaState::DIE:
			velocity.x = 0;
			break;
	}
}

void KoopaTroopa::TakeDamage() {	
	if (hitPoints >= 2) {
		--hitPoints;
		StartRetract();
	}
}

void KoopaTroopa::Update(DWORD delta, std::vector<GameObject*>* objects) {
	if (hitPoints == 0 && !IsBeingRemoved()) {
		StartRemoveTimer();
	}
	
	HandleStates();

	GameObject::Update(delta);

	velocity.y += gravity * delta;

	if (retractStart != 0 && GetTickCount64() - retractStart > retractTime) {
		hitPoints = 3;
		retractStart = 0;
	}

	if (removeStart != 0 && GetTickCount64() - removeStart > removeTime) {
		hitPoints = -1;
		removeStart = 0;
	}

	//shell spinning
	if (hitPoints == 1) {
		StartRetract();
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

			//mario's fireball
			if (dynamic_cast<Entity*>(event->object) && event->object->GetObjectID() == 99) {
				hitPoints = 0;
				scale = D3DXVECTOR2(1.0f, -1.0f);
				velocity.y = -0.23f;
			}

			//koopa shell
			//kill both of them if one is spinning or is held by mario
			if (dynamic_cast<KoopaTroopa*>(event->object) && 
				(dynamic_cast<KoopaTroopa*>(event->object)->GetCurrentHitPoints() == 1 || isBeingHeld))
			{
				KoopaTroopa* koopa = static_cast<KoopaTroopa*>(event->object);
				koopa->SetCurrenHitPoints(0);
				koopa->SetScale(D3DXVECTOR2(1.0f, -1.0f));
				koopa->SetVelocity(D3DXVECTOR3(0, -0.23f, 0));

				hitPoints = 0;
				scale = D3DXVECTOR2(1.0f, -1.0f);
				velocity.y = -0.23f;
			}

			if (dynamic_cast<Entity*>(event->object)) {
				Entity* entity = static_cast<Entity*>(event->object);
				if (hitPoints == 1) {
					if (dynamic_cast<ShinyBrick*>(event->object) && event->normal.y == 0.0f) {
						//entity->SetCurrenHitPoints(-1);
						entity->StartRemoveTimer();
					}
					else {
						entity->TakeDamage();
					}
				}
			}
			
			//shiny brick
			if (dynamic_cast<Entity*>(event->object)) {
				Entity* entity = static_cast<Entity*>(event->object);
				if (entity->GetObjectID() == 103) {
					if (position.x <= entity->GetPosition().x - 5) {
						if (hitPoints != 1) {
							this->normal.x = -1;
						}
					}
					if (position.x + hitBox.GetWidth() >= entity->GetPosition().x + 5 + entity->GetBoxWidth()) {
						if (hitPoints != 1) {
							this->normal.x = 1;
						}
					}
				}
			}

			//one-way platform
			if (dynamic_cast<Tiles*>(event->object)) {
				Tiles* tile = static_cast<Tiles*>(event->object);
				if (tile->GetObjectID() == 205) {
					if (position.x <= tile->GetPosition().x - 5) {
						if (hitPoints != 1) {
							this->normal.x = -1;
						}
					}
					if (position.x + hitBox.GetWidth() >= tile->GetPosition().x + 5 + tile->GetBoxWidth()) {
						if (hitPoints != 1) {
							this->normal.x = 1;
						}
					}
				}
			}

			if (dynamic_cast<Entity*>(event->object) || dynamic_cast<Tiles*>(event->object)) {
				if (event->normal.x != 0.0f) {
					if (hitPoints != 1 || //spinning
						dynamic_cast<Tiles*>(event->object) ||
						dynamic_cast<ShinyBrick*>(event->object) ||
						dynamic_cast<QuestionBlock*>(event->object))
					{
						this->normal.x = -event->normal.x;
					}
				}
			}
		}
	}

	for (LPCOLLISIONEVENT event : collisionEvents) {
		delete event;
	}
}

void KoopaTroopa::Render() {
	switch (currentState) {
		case KoopaState::WALK:
			if (extraData.size() > 0 && extraData.at(0) == "g") {
				sprite.PlayAnimation("WalkGreen", position, D3DXVECTOR2(normal.x, 1.0f));
			}
			else {
				sprite.PlayAnimation("WalkRed", position, D3DXVECTOR2(normal.x, 1.0f));
			}
			break;		
		case KoopaState::RETRACT:			
			if (extraData.size() > 0 && extraData.at(0) == "g") {
				sprite.PlayAnimation("RetractGreen", D3DXVECTOR3(position.x, position.y + 10, 0), scale);
			}
			else {
				sprite.PlayAnimation("RetractRed", D3DXVECTOR3(position.x, position.y + 10, 0), scale);
			}
			
			if (GetTickCount64() - retractStart > (retractTime * 3 / 4)) {
				if (extraData.size() > 0 && extraData.at(0) == "g") {
					sprite.PlayAnimation("OutGreen", D3DXVECTOR3(position.x, position.y + 10, 0), scale);
				}
				else {
					sprite.PlayAnimation("OutRed", D3DXVECTOR3(position.x, position.y + 10, 0), scale);
				}
			}
			break;
		case KoopaState::SPIN:
			if (extraData.size() > 0 && extraData.at(0) == "g") {
				sprite.PlayAnimation("SpinGreen", D3DXVECTOR3(position.x, position.y - 1, 0), scale);
			}
			else {
				sprite.PlayAnimation("SpinRed", D3DXVECTOR3(position.x, position.y - 1, 0), scale);
			}
			break;
		case KoopaState::DIE:
			if (extraData.size() > 0 && extraData.at(0) == "g") {
				sprite.PlayAnimation("RetractGreen", position, scale);
			}
			else {
				sprite.PlayAnimation("RetractRed", position, scale);
			}
			sprite.PlayAnimation("100", position);
			break;
	}
}

void KoopaTroopa::Release() {
	
}
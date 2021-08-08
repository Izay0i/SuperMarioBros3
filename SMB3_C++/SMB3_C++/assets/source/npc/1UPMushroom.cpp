#include "../../headers/npc/1UPMushroom.h"

LPCWSTR GMushroom::texturePath = nullptr;
LPDIRECT3DTEXTURE9 GMushroom::texture = nullptr;
D3DCOLOR GMushroom::colorKey = D3DCOLOR_XRGB(0, 0, 0);

GMushroom::GMushroom() {
	hitPoints = 1;
	normal = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	score = 16000;
}

void GMushroom::LoadTexture() {
	if (!texture) {
		HRESULT hResult;
		D3DXIMAGE_INFO imageInfo;

		hResult = D3DXGetImageInfoFromFile(texturePath, &imageInfo);
		if (hResult != D3D_OK) {
			OutputDebugStringA("[COIN] Failed to get image info\n");
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

RECTF GMushroom::GetBoundingBox(int id) const {
	RECTF bound;

	if (hitPoints > 0) {
		bound.left = position.x;
		bound.top = position.y;
		bound.right = position.x + hitBox.GetWidth(id);
		bound.bottom = position.y + hitBox.GetHeight(id);
	}

	return bound;
}

void GMushroom::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void GMushroom::ParseHitboxes(std::string line) {
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

void GMushroom::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::vector<std::string> extraData) {
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

void GMushroom::TakeDamage() {
	if (hitPoints > 0) {
		--hitPoints;
		tookDamage = true;
	}

	if (hitPoints == 0) {
		StartRemoveTimer();
	}
}

void GMushroom::Update(DWORD delta, std::vector<GameObject*>* objects) {
	GameObject::Update(delta);

	if (emergeStart != 0 && GetTickCount64() - emergeStart <= emergeTime) {
		velocity.y -= 0.00005f * delta;
	}
	else {
		velocity.x = runSpeed * normal.x * delta;
		velocity.y += gravity * delta;
	}

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
		D3DXVECTOR2 offSet(0.4f, 0.4f);
		D3DXVECTOR3 normal;
		D3DXVECTOR3 relativeDistance;

		FilterCollision(collisionEvents, eventResults, minTime, normal, relativeDistance);

		for (LPCOLLISIONEVENT result : eventResults) {
			LPCOLLISIONEVENT event = result;

			//isOnGround true when:
			//on tiles
			//on portals
			//on question blocks
			//on moving platforms
			//on shiny bricks if their hp != 3
			if ((dynamic_cast<Tiles*>(event->object) ||
				dynamic_cast<Portal*>(event->object) ||
				dynamic_cast<QuestionBlock*>(event->object) ||
				dynamic_cast<MovingPlatform*>(event->object)) &&
				event->normal.y == -1.0f ||
				(dynamic_cast<ShinyBrick*>(event->object) &&
					dynamic_cast<ShinyBrick*>(event->object)->GetCurrentHitPoints() != 3))
			{
				isOnGround = true;
			}
			
			//death
			if (dynamic_cast<Tiles*>(event->object) && event->object->GetObjectID() == 666) {
				position.y = 1000;
				StartRemoveTimer();
			}

			//mushroom
			if (dynamic_cast<SuperMushroom*>(event->object)) {
				minTime.x = 1.0f;
				offSet.x = normal.x = relativeDistance.x = 0.0f;
				if (!isOnGround) {
					minTime.y = 1.0f;
					offSet.y = normal.y = relativeDistance.y = 0.0f;
				}
			}

			//1up shroom
			if (dynamic_cast<GMushroom*>(event->object)) {
				minTime.x = 1.0f;
				offSet.x = normal.x = relativeDistance.x = 0.0f;
				if (!isOnGround) {
					minTime.y = 1.0f;
					offSet.y = normal.y = relativeDistance.y = 0.0f;
				}
			}

			//leaf
			if (dynamic_cast<SuperLeaf*>(event->object)) {
				minTime.x = 1.0f;
				offSet.x = normal.x = relativeDistance.x = 0.0f;
				if (!isOnGround) {
					minTime.y = 1.0f;
					offSet.y = normal.y = relativeDistance.y = 0.0f;
				}
			}

			//coin
			if (dynamic_cast<Coin*>(event->object) && dynamic_cast<Coin*>(event->object)->GetCurrentHitPoints() != 3) {
				minTime.x = 1.0f;
				offSet.x = normal.x = relativeDistance.x = 0.0f;
				if (!isOnGround) {
					minTime.y = 1.0f;
					offSet.y = normal.y = relativeDistance.y = 0.0f;
				}
			}

			//switch side when collides when tiles
			if (dynamic_cast<Tiles*>(event->object) && event->normal.x != 0.0f) {
				this->normal.x = -this->normal.x;
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

void GMushroom::Render() {
	sprite.PlayAnimation("Default", position);

	if (removeStart != 0 && GetTickCount64() - removeStart > removeTime / 2) {
		sprite.PlayAnimation(ScoreToString(score), D3DXVECTOR3(position.x + 1, position.y - 16, 0));
	}
}

void GMushroom::Release() {
	OutputDebugStringA("1UP released\n");
}
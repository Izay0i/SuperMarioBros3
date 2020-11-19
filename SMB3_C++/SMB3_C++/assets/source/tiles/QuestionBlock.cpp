#include "../../headers/tiles/QuestionBlock.h"

LPCWSTR QuestionBlock::texturePath = nullptr;
LPDIRECT3DTEXTURE9 QuestionBlock::texture = nullptr;
D3DCOLOR QuestionBlock::colorKey = D3DCOLOR_XRGB(0, 0, 0);

QuestionBlock::QuestionBlock() {
	//1 - no items
	//2 - rotate sprite
	hitPoints = 2;
}

void QuestionBlock::LoadTexture() {
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

void QuestionBlock::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void QuestionBlock::ParseHitboxes(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 4) {
		return;
	}

	unsigned int left = std::stoul(tokens.at(0));
	unsigned int top = std::stoul(tokens.at(1));
	unsigned int right = std::stoul(tokens.at(2));
	unsigned int bottom = std::stoul(tokens.at(3));

	RECTF hitbox;
	hitbox.left = left;
	hitbox.top = top;
	hitbox.right = right;
	hitbox.bottom = bottom;

	this->hitBox.AddHitBox(hitbox);
}

void QuestionBlock::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey) {
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

void QuestionBlock::HandleStates() {
	switch (hitPoints) {
		case 1:
			currentState = BlockState::PUSHED;
			break;
		case 2:
			currentState = BlockState::ROTATE;
			break;
	}
}

void QuestionBlock::TakeDamage() {
	if (hitPoints > 1) {
		--hitPoints;
		velocity.y = -jumpSpeed;
	}
}

void QuestionBlock::Update(DWORD delta, std::vector<GameObject*>* objects) {
	HandleStates();

	GameObject::Update(delta);
	
	if (position.y < originalPos.y) {
		velocity.y += gravity * delta;
	}
	else {
		velocity.y = 0;
		if (position.y >= originalPos.y) {
			position = originalPos;			
		}
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

			/*if (dynamic_cast<Entity*>(event->object)) {
				Entity* entity = static_cast<Entity*>(event->object);
				if (entity->GetObjectID() == 3) {
					if (event->normal.x != 0.0f) {
						if (entity->GetCurrentHitPoints() == 1) {
							TakeDamage();
						}
					}
				}
			}*/
		}
	}

	for (LPCOLLISIONEVENT event : collisionEvents) {
		delete event;
	}
}

void QuestionBlock::Render() {
	switch (currentState) {
		case BlockState::PUSHED:
			sprite.PlayAnimation("None", position);
			break;
		case BlockState::ROTATE:
			sprite.PlayAnimation("Rotate", position);
			break;
	}
}

void QuestionBlock::Release() {
	
}
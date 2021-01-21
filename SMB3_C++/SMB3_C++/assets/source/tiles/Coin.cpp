#include "../../headers/tiles/Coin.h"

LPCWSTR Coin::texturePath = nullptr;
LPDIRECT3DTEXTURE9 Coin::texture = nullptr;
D3DCOLOR Coin::colorKey = D3DCOLOR_XRGB(0, 0, 0);

Coin::Coin() {
	//1 - rotate
	//2 - question block
	//3 - switched to brick
	hitPoints = 1;
}

void Coin::LoadTexture() {
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

void Coin::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void Coin::ParseHitboxes(std::string line) {
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

void Coin::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::vector<std::string> extraData) {
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

void Coin::HandleStates() {
	switch (hitPoints) {
		case 1:
			currentState = ItemState::ROTATE;
			score = 50;
			break;
		case 2:
			currentState = ItemState::POPPEDOUTOFQBLOCK;
			score = 100;
			break;
		case 3:
			currentState = ItemState::SWITCHEDTOBRICK;
			break;
	}
}

void Coin::TakeDamage() {
	if (hitPoints > 0) {
		--hitPoints;
		tookDamage = true;
	}

	if (hitPoints == 0) {
		StartRemoveTimer();
	}
}

void Coin::Update(DWORD delta, std::vector<GameObject*>* objects) {
	HandleStates();
	
	if (removeStart != 0 && GetTickCount64() - removeStart > removeTime) {
		hitPoints = -1;
		removeStart = 0;
	}

	std::vector<LPCOLLISIONEVENT> collisionEvents, eventResults;
	collisionEvents.clear();

	if (hitPoints == 2) {
		GameObject::Update(delta);
		velocity.y += gravity * delta;

		position += distance;
	}

	if (hitPoints > 0) {
		CalcPotentialCollision(objects, collisionEvents);
	}

	D3DXVECTOR2 minTime;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 relativeDistance;

	FilterCollision(collisionEvents, eventResults, minTime, normal, relativeDistance);

	for (LPCOLLISIONEVENT result : eventResults) {
		LPCOLLISIONEVENT event = result;

		if (event->normal.y == -1.0f) {
			if (hitPoints == 2) {
				tookDamage = true;
				StartRemoveTimer();
			}
		}
	}

	for (LPCOLLISIONEVENT event : collisionEvents) {
		delete event;
	}
}

void Coin::Render() {
	switch (currentState) {
		case ItemState::ROTATE:
			sprite.PlayAnimation("Rotate", position);
			break;
		case ItemState::POPPEDOUTOFQBLOCK:
			sprite.PlayAnimation("Pushed", position);
			break;
		case ItemState::SWITCHEDTOBRICK:
			sprite.PlayAnimation("Brick", position);
			break;
	}

	if (removeStart != 0 && GetTickCount64() - removeStart > removeTime / 2 && hitPoints == 2) {
		sprite.PlayAnimation(ScoreToString(score), D3DXVECTOR3(position.x + 1, position.y + 6, 0));
	}
}

void Coin::Release() {
	OutputDebugStringA("Coin released\n");
}
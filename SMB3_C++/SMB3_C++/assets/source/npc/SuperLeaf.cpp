#include "../../headers/npc/SuperLeaf.h"

LPCWSTR SuperLeaf::texturePath = nullptr;
LPDIRECT3DTEXTURE9 SuperLeaf::texture = nullptr;
D3DCOLOR SuperLeaf::colorKey = D3DCOLOR_XRGB(0, 0, 0);

SuperLeaf::SuperLeaf() {
	hitPoints = 1;
	normal = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	scale = D3DXVECTOR2(1.0f, 1.0f);
	score = 1000;
}

void SuperLeaf::LoadTexture() {
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

RECTF SuperLeaf::GetBoundingBox(int id) const {
	RECTF bound;

	if (hitPoints > 0) {
		bound.left = position.x;
		bound.top = position.y;
		bound.right = position.x + hitBox.GetWidth(id);
		bound.bottom = position.y + hitBox.GetHeight(id);
	}

	return bound;
}

void SuperLeaf::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void SuperLeaf::ParseHitboxes(std::string line) {
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

void SuperLeaf::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::vector<std::string> extraData) {
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

void SuperLeaf::TakeDamage() {
	if (hitPoints > 0) {
		--hitPoints;
		tookDamage = true;
	}

	if (hitPoints == 0) {
		StartRemoveTimer();
	}
}

void SuperLeaf::Update(DWORD delta, std::vector<GameObject*>* objects) {
	std::uniform_real_distribution<float> dist(0.0f, 1.2f);
	
	GameObject::Update(delta);
	
	velocity.y += gravity * delta;

	position.x += sin(distance.x) + dist(randDev) * normal.x;
	position.y += distance.y;

	if (GetTickCount64() % 1000 == 0) {
		scale.x = -scale.x;
		normal.x = -normal.x;
	}

	if (removeStart != 0 && GetTickCount64() - removeStart > removeTime) {
		hitPoints = -1;
		removeStart = 0;
	}

	std::vector<LPCOLLISIONEVENT> collisionEvents, eventResults;
	
	CalcPotentialCollision(objects, collisionEvents);

	D3DXVECTOR2 minTime;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 relativeDistance;

	FilterCollision(collisionEvents, eventResults, minTime, normal, relativeDistance);

	for (LPCOLLISIONEVENT result : eventResults) {
		LPCOLLISIONEVENT event = result;

		//death
		if (dynamic_cast<Tiles*>(event->object) && event->object->GetObjectID() == 666) {
			position.y = 1000;
			StartRemoveTimer();
		}
	}

	for (LPCOLLISIONEVENT event : collisionEvents) {
		delete event;
	}
}

void SuperLeaf::Render() {
	sprite.PlayAnimation("Default", position, scale);

	if (removeStart != 0 && GetTickCount64() - removeStart > removeTime / 2) {
		sprite.PlayAnimation(ScoreToString(score), D3DXVECTOR3(position.x + 1, position.y + 6, 0));
	}
}

void SuperLeaf::Release() {
	OutputDebugStringA("Leaf released\n");
}
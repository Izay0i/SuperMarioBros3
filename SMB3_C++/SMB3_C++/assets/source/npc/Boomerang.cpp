#include "../../headers/npc/Boomerang.h"

LPCWSTR Boomerang::texturePath = nullptr;
LPDIRECT3DTEXTURE9 Boomerang::texture = nullptr;
D3DCOLOR Boomerang::colorKey = D3DCOLOR_XRGB(0, 0, 0);

Boomerang::Boomerang() {
	velocity.x = moveSpeed;
	velocity.y = -0.12f;

	StartRemoveTimer();
}

RECTF Boomerang::GetBoundingBox(int id) const {
	RECTF bound;
	bound.left = position.x;
	bound.top = position.y;
	bound.right = position.x + hitBox.GetWidth(id);
	bound.bottom = position.y + hitBox.GetHeight(id);

	return bound;
}

void Boomerang::LoadTexture() {
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

void Boomerang::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void Boomerang::ParseHitboxes(std::string line) {
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

void Boomerang::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::vector<std::string> extraData) {
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

void Boomerang::Update(DWORD delta, std::vector<GameObject*>* objects) {
	/*char debug[100];
	sprintf_s(debug, "Velx: %f\n", velocity.x);
	OutputDebugStringA(debug);*/

	if (removeStart != 0 && GetTickCount64() - removeStart > removeTime) {
		hitPoints = -1;
		removeStart = 0;
	}

	GameObject::Update(delta);
	position += distance;

	if (velocity.x > 0) {
		velocity.y += 0.0002f * delta;
		if (position.y >= originalPos.y - 4) {
			velocity.x -= 0.0002f * delta;
			if (velocity.x <= moveSpeed) {
				velocity.x = -moveSpeed;
			}
		}
	}
	else if (velocity.x < 0) {
		velocity.y -= 0.0002f * delta;
		if (position.y <= originalPos.y + 4) {
			velocity.x += 0.0002f * delta;
			if (velocity.x <= moveSpeed) {
				velocity.x = -moveSpeed;
			}
		}
	}

	if (position.x - originalPos.x >= MAX_DISTANCE && velocity.x > 0) {
		velocity.y = 0.1f;
	}
	else if (position.x <= originalPos.x && velocity.x < 0) {
		velocity.y = -0.1f;
		velocity.x = moveSpeed;
	}
}

void Boomerang::Render() {
	sprite.PlayAnimation("Spin", position);
}

void Boomerang::Release() {

}
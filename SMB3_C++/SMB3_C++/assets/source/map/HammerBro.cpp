#include "../../headers/map/HammerBro.h"

LPCWSTR HammerBro::texturePath = nullptr;
LPDIRECT3DTEXTURE9 HammerBro::texture = nullptr;
D3DCOLOR HammerBro::colorKey = D3DCOLOR_XRGB(0, 0, 0);

HammerBro::HammerBro() {
	normal = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
}

RECTF HammerBro::GetBoundingBox(int id) const {
	return hitBox.GetBoundingBox(0);
}

void HammerBro::LoadTexture() {
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

void HammerBro::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void HammerBro::ParseHitboxes(std::string line) {
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

void HammerBro::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::vector<std::string> extraData) {
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

void HammerBro::Update(DWORD delta, std::vector<GameObject*>* objects) {
	velocity.x = 0.02f * normal.x;
	GameObject::Update(delta);
	position += distance;

	if (position.x <= originalPos.x - MIN_X_OFFSET) {
		normal.x = 1.0f;
	}
	else if (position.x >= originalPos.x + MAX_X_OFFSET) {
		normal.x = -1.0f;
	}
}

void HammerBro::Render() {
	sprite.PlayAnimation("Walk", position, D3DXVECTOR2(normal.x, 1.0f));
}

void HammerBro::Release() {

}
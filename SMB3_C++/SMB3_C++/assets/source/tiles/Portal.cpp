#include "../../headers/tiles/Portal.h"

LPCWSTR Portal::texturePath = nullptr;
LPDIRECT3DTEXTURE9 Portal::texture = nullptr;
D3DCOLOR Portal::colorKey = D3DCOLOR_XRGB(0, 0, 0);

RECTF Portal::GetBoundingBox(int id) const {
	RECTF bound;
	
	if (extraData.size() > 1) {
		bound.left = position.x + 2;
		bound.top = position.y + 1;
		bound.right = position.x + hitBox.GetWidth(id) - 3;
		bound.bottom = position.y + hitBox.GetHeight(id) - 5;
	}
	else {
		bound.left = position.x;
		bound.top = position.y;
		bound.right = position.x + hitBox.GetWidth(1);
		bound.bottom = position.y + hitBox.GetHeight(1);
	}

	return bound;
}

int Portal::GetNextSceneID() const {
	return std::stoi(extraData.front());
}

D3DXVECTOR3 Portal::GetDestination() const {
	return destination;
}

void Portal::LoadTexture() {
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

void Portal::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void Portal::ParseHitboxes(std::string line) {
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

void Portal::ParseExtraData(std::vector<std::string> extraData) {
	if (extraData.size() > 1) {
		float posX = std::stof(extraData.at(0));
		float posY = std::stof(extraData.at(1));

		destination = D3DXVECTOR3(posX, posY, 0);
	}
}

void Portal::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::vector<std::string> extraData) {
	std::ifstream readFile;
	readFile.open(dataPath, std::ios::in);

	if (!readFile.is_open()) {
		OutputDebugStringA("Failed to read data\n");
		return;
	}

	if (extraData.size() > 0) {
		this->extraData = extraData;
		ParseExtraData(extraData);
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

void Portal::Update(DWORD delta, std::vector<GameObject*>* objects) {
	/*char debug[100];
	sprintf_s(debug, "Pos: %f %f\n", position.x, position.y);
	OutputDebugStringA(debug);*/
}

void Portal::Render() {

}

void Portal::Release() {

}
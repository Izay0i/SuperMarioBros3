#include "../../headers/tiles/BonusItem.h"

LPCWSTR BonusItem::texturePath = nullptr;
LPDIRECT3DTEXTURE9 BonusItem::texture = nullptr;
D3DCOLOR BonusItem::colorKey = D3DCOLOR_XRGB(0, 0, 0);

BonusItem::BonusItem() {
	
}

void BonusItem::LoadTexture() {
	if (!texture) {
		HRESULT hResult;
		D3DXIMAGE_INFO imageInfo;

		hResult = D3DXGetImageInfoFromFile(texturePath, &imageInfo);
		if (hResult != D3D_OK) {
			OutputDebugStringA("[BONUS ITEM] Failed to get image info\n");
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

void BonusItem::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void BonusItem::ParseHitboxes(std::string line) {
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

void BonusItem::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey) {
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

void BonusItem::Update(DWORD delta, std::vector<GameObject*>* objects) {
	
}

void BonusItem::Render() {
	sprite.PlayAnimation("Rotate", position);
}

void BonusItem::Release() {

}
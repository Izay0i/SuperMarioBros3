#include "../../headers/tiles/SwitchBlock.h"

LPCWSTR SwitchBlock::texturePath = nullptr;
LPDIRECT3DTEXTURE9 SwitchBlock::texture = nullptr;
D3DCOLOR SwitchBlock::colorKey = D3DCOLOR_XRGB(0, 0, 0);

SwitchBlock::SwitchBlock() {
	//1 - inactive
	//2 - active
	hitPoints = 2;
}

void SwitchBlock::LoadTexture() {
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

void SwitchBlock::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void SwitchBlock::ParseHitboxes(std::string line) {
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

void SwitchBlock::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey) {
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

void SwitchBlock::HandleStates() {
	switch (hitPoints) {
		case 1:
			currentState = BlockState::INACTIVE;
			break;
		case 2:
			currentState = BlockState::ACTIVE;
			break;
	}
}

void SwitchBlock::TakeDamage() {
	if (hitPoints > 1) {
		--hitPoints;
	}
}

void SwitchBlock::Update(DWORD delta, std::vector<GameObject*>* objects) {
	HandleStates();
}

void SwitchBlock::Render() {
	switch (currentState) {
		case BlockState::INACTIVE:
			sprite.PlayAnimation("Inactive", position);
			break;
		case BlockState::ACTIVE:
			sprite.PlayAnimation("Active", position);
			break;
	}
}

void SwitchBlock::Release() {

}
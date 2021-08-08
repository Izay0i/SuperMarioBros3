#include "../../headers/tiles/SwitchBlock.h"

LPCWSTR SwitchBlock::texturePath = nullptr;
LPDIRECT3DTEXTURE9 SwitchBlock::texture = nullptr;
D3DCOLOR SwitchBlock::colorKey = D3DCOLOR_XRGB(0, 0, 0);

SwitchBlock::SwitchBlock() {
	//1 - active
	//2 - inactive
	hitPoints = 2;
	score = 0;
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

void SwitchBlock::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::vector<std::string> extraData) {
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

void SwitchBlock::HandleStates() {
	switch (hitPoints) {
		case 1:
			currentState = BlockState::ACTIVE;
			break;
		case 2:
			currentState = BlockState::INACTIVE;
			break;
	}
}

void SwitchBlock::TakeDamage() {
	if (hitPoints > 1) {
		--hitPoints;
		StartActivationTimer();
	}
}

void SwitchBlock::Update(DWORD delta, std::vector<GameObject*>* objects) {
	HandleStates();

	GameObject::Update(delta);

	if (activateStart != 0 && GetTickCount64() - activateStart > activateTime) {
		activateStart = 0;
		OutputDebugStringA("Activated\n");
	}

	std::vector<Coin*> coins;
	std::vector<ShinyBrick*> bricks;

	for (unsigned int i = 0; i < objects->size(); ++i) {
		if (dynamic_cast<Entity*>(objects->at(i))) {
			if (dynamic_cast<Coin*>(objects->at(i))) {
				Coin* coin = static_cast<Coin*>(objects->at(i));
				coins.push_back(coin);
			}
			else if (dynamic_cast<ShinyBrick*>(objects->at(i))) {
				ShinyBrick* shinyBrick = static_cast<ShinyBrick*>(objects->at(i));
				bricks.push_back(shinyBrick);
			}
		}
	}

	if (IsActivated()) {
		for (Coin* coin : coins) {
			if (coin->GetCurrentHitPoints() == 1) {
				coin->SetCurrenHitPoints(3);
			}
		}

		for (ShinyBrick* brick : bricks) {
			if (brick->GetCurrentHitPoints() == 2 && brick->GetExtraDataSize() == 0) {
				brick->SetCurrenHitPoints(3);
			}
		}
	}
	else {
		for (Coin* coin : coins) {
			if (coin->GetCurrentHitPoints() == 3) {
				coin->SetCurrenHitPoints(1);
			}
		}

		for (ShinyBrick*& brick : bricks) {
			if (brick->GetCurrentHitPoints() == 3) {
				brick->SetCurrenHitPoints(2);
			}
		}
	}
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
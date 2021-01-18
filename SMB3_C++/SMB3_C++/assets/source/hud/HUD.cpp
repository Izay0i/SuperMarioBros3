#include "../../headers/hud/HUD.h"

HUD* HUD::hudInstance = nullptr;
LPCWSTR HUD::texturePath = nullptr;
LPDIRECT3DTEXTURE9 HUD::texture = nullptr;
D3DCOLOR HUD::colorKey = D3DCOLOR_XRGB(0, 0, 0);

HUD* HUD::GetInstance() {
	if (!hudInstance) {
		hudInstance = new HUD;
	}

	return hudInstance;
}

void HUD::LoadTexture() {
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

void HUD::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void HUD::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::vector<std::string> extraData) {
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

		switch (section) {
		case DataSection::DATA_SECTION_SPRITES:
			ParseSprites(line);
			break;
		}
	}

	readFile.close();
}

void HUD::ParseLives(unsigned int lives) {
	std::vector<unsigned int> digits = Util::splitDigit(lives);
	std::vector<std::string> tokens;

	for (unsigned int i = 0; i < digits.size(); ++i) {
		tokens.push_back(std::to_string(digits.at(i)));
	}

	if (tokens.size() < LIVES_MAX_LENGTH) {
		for (unsigned int i = 0, end = LIVES_MAX_LENGTH - tokens.size(); i < end; ++i) {
			tokens.insert(tokens.begin(), "0");
		}
	}

	livesArr.clear();
	for (unsigned int i = 0; i < tokens.size(); ++i) {
		livesArr.push_back(DigitToString(tokens.at(i)));
	}
}

void HUD::ParseCoins(unsigned int coins) {
	std::vector<unsigned int> digits = Util::splitDigit(coins);
	std::vector<std::string> tokens;

	for (unsigned int i = 0; i < digits.size(); ++i) {
		tokens.push_back(std::to_string(digits.at(i)));
	}

	if (tokens.size() < COINS_MAX_LENGTH) {
		for (unsigned int i = 0, end = COINS_MAX_LENGTH - tokens.size(); i < end; ++i) {
			tokens.insert(tokens.begin(), "0");
		}
	}

	coinsArr.clear();
	for (unsigned int i = 0; i < tokens.size(); ++i) {
		coinsArr.push_back(DigitToString(tokens.at(i)));
	}
}

void HUD::ParseItems(std::vector<Entity::ObjectType> items) {
	if (items.size() == 0) {
		return;
	}
	
	itemsArr.clear();
	for (unsigned int i = 0; i < items.size(); ++i) {
		switch (items.at(i)) {
			case Entity::ObjectType::OBJECT_TYPE_MUSHROOM:
				itemsArr.push_back("Mushroom");
				break;
			case Entity::ObjectType::OBJECT_TYPE_FLOWER:
				itemsArr.push_back("Flower");
				break;
			case Entity::ObjectType::OBJECT_TYPE_STAR:
				itemsArr.push_back("Star");
				break;
		}
	}
}

void HUD::ParseSpeedGauge(float currentAccel, float maxAccel, bool isFlying, bool keyPressed) {
	speedGaugeArr.clear();

	if (keyPressed || isFlying) {
		if (currentAccel >= 1.5f / maxAccel || isFlying) {
			speedGaugeArr.push_back("Arrow");
		}

		if (currentAccel >= 1.75f / maxAccel || isFlying) {
			speedGaugeArr.push_back("Arrow");
		}

		if (currentAccel >= 2.0f / maxAccel || isFlying) {
			speedGaugeArr.push_back("Arrow");
		}

		if (currentAccel >= 2.5f / maxAccel || isFlying) {
			speedGaugeArr.push_back("Arrow");
		}

		if (currentAccel >= 3.0f / maxAccel || isFlying) {
			speedGaugeArr.push_back("Arrow");
		}

		if (currentAccel >= 3.5f / maxAccel || isFlying) {
			speedGaugeArr.push_back("Arrow");
		}

		if (currentAccel >= 4.0f / maxAccel || isFlying) {
			speedGaugeArr.push_back("PNode");
		}
	}
}

void HUD::ParseScore(unsigned int score) {
	std::vector<unsigned int> digits = Util::splitDigit(score);
	std::vector<std::string> tokens;

	for (unsigned int i = 0; i < digits.size(); ++i) {
		tokens.push_back(std::to_string(digits.at(i)));
	}

	if (tokens.size() < SCORE_MAX_LENGTH) {
		for (unsigned int i = 0, end = SCORE_MAX_LENGTH - tokens.size(); i < end; ++i) {
			tokens.insert(tokens.begin(), "0");
		}
	}

	scoreArr.clear();
	for (unsigned int i = 0; i < tokens.size(); ++i) {
		scoreArr.push_back(DigitToString(tokens.at(i)));
	}
}

void HUD::ParseTime(unsigned long time) {
	std::vector<unsigned int> digits = Util::splitDigit(time);
	std::vector<std::string> tokens;

	for (unsigned int i = 0; i < digits.size(); ++i) {
		tokens.push_back(std::to_string(digits.at(i)));
	}

	if (tokens.size() < TIME_MAX_LENGTH) {
		for (unsigned int i = 0, end = TIME_MAX_LENGTH - tokens.size(); i < end; ++i) {
			tokens.insert(tokens.begin(), "0");
		}
	}

	timeArr.clear();
	for (unsigned int i = 0; i < tokens.size(); ++i) {
		timeArr.push_back(DigitToString(tokens.at(i)));
	}
}

void HUD::ParseSceneEnd(std::vector<Entity::ObjectType> items) {
	if (items.size() == 0) {
		return;
	}
	
	Entity::ObjectType item = items.back();

	switch (item) {
		case Entity::ObjectType::OBJECT_TYPE_MUSHROOM:
			animName = "Mushroom";
			break;
		case Entity::ObjectType::OBJECT_TYPE_FLOWER:
			animName = "Flower";
			break;
		case Entity::ObjectType::OBJECT_TYPE_STAR:
			animName = "Star";
			break;
	}
}

std::string HUD::DigitToString(std::string digitStr) {
	std::string animName = "Error";
	unsigned int digit = std::stoi(digitStr);

	switch (digit) {
		case 0:
			animName = "0";
			break;
		case 1:
			animName = "1";
			break;
		case 2:
			animName = "2";
			break;
		case 3:
			animName = "3";
			break;
		case 4:
			animName = "4";
			break;
		case 5:
			animName = "5";
			break;
		case 6:
			animName = "6";
			break;
		case 7:
			animName = "7";
			break;
		case 8:
			animName = "8";
			break;
		case 9:
			animName = "9";
			break;
	}

	return animName;
}

void HUD::Update(
	DWORD delta, 
	unsigned int lives, 
	unsigned int coins, 
	std::vector<Entity::ObjectType> items, 
	float currentAccel, 
	float maxAccel, 
	unsigned int score, 
	unsigned long time,
	bool iSFlying,
	bool keyPressed,
	bool sceneEnded) 
{
	ParseLives(lives);
	ParseCoins(coins);
	ParseItems(items);
	ParseSpeedGauge(currentAccel, maxAccel, iSFlying, keyPressed);
	ParseScore(score);
	ParseTime(time);
	ParseSceneEnd(items);

	this->sceneEnded = sceneEnded;
}

void HUD::Render() {
	//Item panel:
	//First panel: offset x: 161
	//Second panel: offset x: 185
	//Third panel: offset x: 209

	//Order of rendering:
	//Panel
	//Lives
	//Coins
	//Items
	//Speed Gauge
	//Score
	//Timer

	sprite.PlayAnimation("Bar", D3DXVECTOR3(position.x - 6, position.y - 3, 0));
	sprite.PlayAnimation("Panel", position);

	//Lives
	for (unsigned int i = 0; i < livesArr.size(); ++i) {
		//(8 + i) -> x offset
		sprite.PlayAnimation(livesArr.at(i), D3DXVECTOR3(position.x + 30 + (8 * i), position.y + 16, 0));
	}

	//Coins
	for (unsigned int i = 0; i < coinsArr.size(); ++i) {
		sprite.PlayAnimation(coinsArr.at(i), D3DXVECTOR3(position.x + 133 + (8 * i), position.y + 8, 0));
	}

	//Items
	for (unsigned int i = 0; i < itemsArr.size(); ++i) {
		sprite.PlayAnimation(itemsArr.at(i), D3DXVECTOR3(position.x + 161 + (24 * i), position.y, 0));
	}

	//Speed Gauge
	for (unsigned int i = 0; i < speedGaugeArr.size(); ++i) {
		sprite.PlayAnimation(speedGaugeArr.at(i), D3DXVECTOR3(position.x + 53 + (8 * i), position.y + 8, 0));
	}

	//Score
	for (unsigned int i = 0; i < scoreArr.size(); ++i) {
		sprite.PlayAnimation(scoreArr.at(i), D3DXVECTOR3(position.x + 53 + (8 * i), position.y + 16, 0));
	}

	//Time
	for (unsigned int i = 0; i < timeArr.size(); ++i) {
		sprite.PlayAnimation(timeArr.at(i), D3DXVECTOR3(position.x + 125 + (8 * i), position.y + 16, 0));
	}

	if (sceneEnded) {
		sprite.PlayAnimation("CourseClear", D3DXVECTOR3(position.x + 60, position.y - 170, 0));
		sprite.PlayAnimation(animName, D3DXVECTOR3(position.x + 183, position.y - 155, 0));
	}	
}

void HUD::Release() {
	if (hudInstance) {
		if (texturePath) {
			delete texturePath;
			texturePath = nullptr;
		}

		if (texture) {
			texture->Release();
			texture = nullptr;
		}

		delete hudInstance;
		hudInstance = nullptr;
	}
}
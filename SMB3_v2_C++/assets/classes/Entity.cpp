#include "Entity.h"

Entity::Entity() {
	_health = 1;
	_score = 100;
	_scoreMultiplier = 1;
}

Entity::~Entity() {}

void Entity::SetHealth(int health) {
	if (health > -2) {
		_health = health;
	}
}

int Entity::GetHealth() const {
	return _health;
}

void Entity::ParseHitboxes(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 4) {
		return;
	}

	RECTF hitbox;
	hitbox.left = std::stof(tokens.at(0));
	hitbox.top = std::stof(tokens.at(1));
	hitbox.right = std::stof(tokens.at(2));
	hitbox.bottom = std::stof(tokens.at(4));
	
	_hitbox.AddHitbox(hitbox);
}

void Entity::ParseData(
	std::string dataPath, 
	std::string texturePath, 
	D3DCOLOR colorKey, 
	std::vector<std::string> extraData) 
{
	std::ifstream readFile;
	readFile.open(dataPath, std::ios::in);

	if (!readFile.is_open()) {
		OutputDebugStringA("[ENTITY] Failed to read data\n");
		return;
	}

	if (extraData.size() > 0) {
		_extraData = extraData;
	}

	_texturePath = GlobalUtil::ToLPCWSTR(texturePath);
	_colorKey = colorKey;

	LoadTexture();

	_DataFileSection dataFileSection = _DataFileSection::DATAFILE_SECTION_UNKNOWN;
	char str[GlobalUtil::MAX_FILE_LINE];
	while (readFile.getline(str, GlobalUtil::MAX_FILE_LINE)) {
		std::string line(str);

		if (line.empty() || line.front() == '#') {
			continue;
		}

		if (line == "[SPRITES]") {
			dataFileSection = _DataFileSection::DATAFILE_SECTION_SPRITES;
			continue;
		}

		if (line == "[HITBOXES]") {
			dataFileSection = _DataFileSection::DATAFILE_SECTION_HITBOXES;
			continue;
		}

		switch (dataFileSection) {
			case _DataFileSection::DATAFILE_SECTION_SPRITES:
				ParseSprites(line);
				break;
			case _DataFileSection::DATAFILE_SECTION_HITBOXES:
				ParseHitboxes(line);
				break;
		}
	}

	readFile.close();
}
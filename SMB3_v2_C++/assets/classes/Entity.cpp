#include "Entity.h"

void Entity::_ParseHitboxes(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 4) {
		return;
	}

	RECTF hitbox;
	hitbox.left = std::stof(tokens.at(0));
	hitbox.top = std::stof(tokens.at(1));
	hitbox.right = std::stof(tokens.at(2));
	hitbox.bottom = std::stof(tokens.at(3));

	_hitbox.AddHitbox(hitbox);
}

bool Entity::CompareRenderPriority(const Entity*& a, const Entity*& b) {
	return a->_renderPriority < b->_renderPriority;
}

Entity::Entity() {
	_health = 1;
	_renderPriority = std::numeric_limits<unsigned int>::max();
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

AnimatedSprite Entity::GetAnimatedSprite() const {
	return _animatedSprite;
}

void Entity::ParseData(
	std::string dataPath, 
	const LPDIRECT3DTEXTURE9& texture,  
	std::vector<std::string> extraData) 
{
	std::ifstream readFile;
	readFile.open(dataPath, std::ios::in);

	if (!readFile.is_open()) {
		OutputDebugStringA("[ENTITY] Failed to read data\n");
		return;
	}

	if (!extraData.empty()) {
		_extraData = extraData;
	}

	_DataFileSection dataFileSection = _DataFileSection::DATAFILE_SECTION_UNKNOWN;
	char str[GlobalUtil::MAX_FILE_LINE];
	while (readFile.getline(str, GlobalUtil::MAX_FILE_LINE)) {
		std::string line(str);

		if (line.empty() || line.front() == '#') {
			continue;
		}

		if (line == "[/]") {
			dataFileSection = _DataFileSection::DATAFILE_SECTION_UNKNOWN;
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
				_ParseSprites(line);
				break;
			case _DataFileSection::DATAFILE_SECTION_HITBOXES:
				_ParseHitboxes(line);
				break;
		}
	}

	readFile.close();
}
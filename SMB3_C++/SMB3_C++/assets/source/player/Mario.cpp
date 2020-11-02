#include "../../headers/player/Mario.h"

Entity* Mario::marioInstance = nullptr;

Mario::Mario() {
	sprite = new AnimatedSprite;
}

Mario* Mario::GetInstance() {
	if (!marioInstance) {
		marioInstance = new Mario;
	}

	return static_cast<Mario*>(marioInstance);
}

void Mario::ParseSprites(std::string line) {
	sprite->ParseSprites(line);
}

void Mario::ParseHitboxes(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 4) {
		return;
	}

	unsigned int right = atoi(tokens.at(2).c_str());
	unsigned int bottom = atoi(tokens.at(3).c_str());

	RECT hitbox;
	hitbox.left = hitbox.top = 0;
	hitbox.right = right;
	hitbox.bottom = bottom;

	this->hitBox.AddHitBox(hitbox);
}

void Mario::ParseData(std::string dataPath) {
	std::ifstream readFile;
	readFile.open(dataPath, std::ios::in);

	if (!readFile.is_open()) {
		OutputDebugStringA("Failed to read data\n");
		return;
	}

	DataSection section = DataSection::DATA_SECTION_UNKNOWN;

	char str[MAX_FILE_LINE];
	while (readFile.getline(str, MAX_FILE_LINE)) {
		std::string line(str);

		if (line[0] == '#' || line.empty()) {
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

void Mario::SetPosition(D3DXVECTOR3 pos) {
	position = pos;
}

D3DXVECTOR3 Mario::GetPosition() {
	return position;
}

void Mario::SetRotation(D3DXVECTOR2 rot) {
	rotation = rot;
}

D3DXVECTOR2 Mario::GetRotation() {
	return rotation;
}

void Mario::SetTranslation(D3DXVECTOR2 trans) {
	translation = trans;
}

D3DXVECTOR2 Mario::GetTranslation() {
	return translation;
}

void Mario::SetScale(D3DXVECTOR2 scale) {
	this->scale = scale;
}

D3DXVECTOR2 Mario::GetScale() {
	return scale;
}

void Mario::CheckCollision(Entity* movingEntity, Entity* staticEntity) {

}

void Mario::Update(DWORD delta) {

}

void Mario::Render() {
	sprite->PlayAnimation("Die");
}

void Mario::Release() {
	if (marioInstance) {
		delete marioInstance;
		marioInstance = nullptr;
	}
}
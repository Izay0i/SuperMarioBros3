//#include "../../headers/tiles/BonusItem.h"
//
//BonusItem::BonusItem() {
//	sprite = new AnimatedSprite;
//}
//
//void BonusItem::ParseSprites(std::string line) {
//	sprite->ParseSprites(line);
//}
//
//void BonusItem::ParseHitboxes(std::string line) {
//	std::vector<std::string> tokens = Util::split(line);
//
//	if (tokens.size() < 4) {
//		return;
//	}
//
//	unsigned int left = atoi(tokens.at(0).c_str());
//	unsigned int top = atoi(tokens.at(1).c_str());
//	unsigned int right = atoi(tokens.at(2).c_str());
//	unsigned int bottom = atoi(tokens.at(3).c_str());
//
//	RECTF hitbox;
//	hitbox.left = left;
//	hitbox.top = top;
//	hitbox.right = right;
//	hitbox.bottom = bottom;
//
//	this->hitBox.AddHitBox(hitbox);
//}
//
//void BonusItem::ParseData(std::string dataPath) {
//	std::ifstream readFile;
//	readFile.open(dataPath, std::ios::in);
//
//	if (!readFile.is_open()) {
//		OutputDebugStringA("Failed to read data\n");
//		return;
//	}
//
//	DataSection section = DataSection::DATA_SECTION_UNKNOWN;
//
//	char str[MAX_FILE_LINE];
//	while (readFile.getline(str, MAX_FILE_LINE)) {
//		std::string line(str);
//
//		if (line[0] == '#' || line.empty()) {
//			continue;
//		}
//
//		if (line == "[SPRITES]") {
//			section = DataSection::DATA_SECTION_SPRITES;
//			continue;
//		}
//
//		if (line == "[HITBOXES]") {
//			section = DataSection::DATA_SECTION_HITBOXES;
//			continue;
//		}
//
//		switch (section) {
//		case DataSection::DATA_SECTION_SPRITES:
//			ParseSprites(line);
//			break;
//		case DataSection::DATA_SECTION_HITBOXES:
//			ParseHitboxes(line);
//			break;
//		}
//	}
//
//	readFile.close();
//}
//
//void BonusItem::Update(DWORD delta) {
//
//}
//
//void BonusItem::Render() {
//	sprite->PlayAnimation("Rotate", position);
//}
//
//void BonusItem::Release() {
//	if (sprite) {
//		delete sprite;
//		sprite = nullptr;
//	}
//}
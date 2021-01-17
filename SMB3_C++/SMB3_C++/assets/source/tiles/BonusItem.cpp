#include "../../headers/tiles/BonusItem.h"

LPCWSTR BonusItem::texturePath = nullptr;
LPDIRECT3DTEXTURE9 BonusItem::texture = nullptr;
D3DCOLOR BonusItem::colorKey = D3DCOLOR_XRGB(0, 0, 0);

BonusItem::BonusItem() {
	//1 - pickedup
	//2 - rotate
	hitPoints = 2;
	score = 0;
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

RECTF BonusItem::GetBoundingBox(int id) const {
	RECTF bound;
	bound.left = position.x;
	bound.top = position.y;
	
	if (hitPoints >= 2) {
		bound.right = position.x + hitBox.GetWidth(id);
		bound.bottom = position.y + hitBox.GetHeight(id);
	}
	else {
		bound.right = position.x + hitBox.GetWidth(1);
		bound.bottom = position.y + hitBox.GetHeight(1);
	}

	return bound;
}

void BonusItem::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void BonusItem::ParseHitboxes(std::string line) {
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

void BonusItem::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::vector<std::string> extraData) {
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

void BonusItem::HandleStates() {
	switch (hitPoints) {
		case 1:
			currentState = ItemState::PICKEDUP;
			break;
		case 2:
			currentState = ItemState::ROTATE;
			break;
	}
}

void BonusItem::TakeDamage() {
	if (hitPoints > 0) {
		--hitPoints;
		StartRemoveTimer();
	}
}

void BonusItem::Update(DWORD delta, std::vector<GameObject*>* objects) {
	HandleStates();
	
	if (removeStart != 0 && GetTickCount64() - removeStart > removeTime) {
		hitPoints = -1;
		removeStart = 0;
	}

	switch (currentState) {
		case ItemState::PICKEDUP:
			velocity.y -= 0.006f;
			GameObject::Update(delta);
			position += distance;
			break;
		case ItemState::ROTATE:
			std::vector<Entity::ObjectType> items = {
				Entity::ObjectType::OBJECT_TYPE_STAR,
				Entity::ObjectType::OBJECT_TYPE_FLOWER,
				Entity::ObjectType::OBJECT_TYPE_MUSHROOM
			};
			
			std::random_device device;
			std::mt19937 rng(device());
			std::uniform_int_distribution<std::mt19937::result_type> dist(0, 2);

			if (GetTickCount64() % 100 == 0) {
				currentItem = items.at(dist(rng));
			}
			break;
	}
}

void BonusItem::Render() {
	switch (currentState) {
		case ItemState::PICKEDUP:
			{
				std::string animName;
				switch (currentItem) {
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
				
				if (GetTickCount64() - removeStart > (removeTime * 3 / 4)) {
					sprite.PlayAnimation("Spark", position);
				}
				else {
					sprite.PlayAnimation(animName, position);
				}
			}
			break;
		case ItemState::ROTATE:
			sprite.PlayAnimation("Rotate", position);
			break;
	}
}

void BonusItem::Release() {

}
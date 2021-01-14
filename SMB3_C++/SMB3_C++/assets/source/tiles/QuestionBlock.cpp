#include "../../headers/tiles/QuestionBlock.h"

LPCWSTR QuestionBlock::texturePath = nullptr;
LPDIRECT3DTEXTURE9 QuestionBlock::texture = nullptr;
D3DCOLOR QuestionBlock::colorKey = D3DCOLOR_XRGB(0, 0, 0);

QuestionBlock::QuestionBlock() {
	//1 - no items
	//2 - rotate sprite
	hitPoints = 2;
}

void QuestionBlock::LoadTexture() {
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

void QuestionBlock::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void QuestionBlock::ParseHitboxes(std::string line) {
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

void QuestionBlock::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::vector<std::string> extraData) {
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

void QuestionBlock::GetMarioCurrentHP(int currentHP) {
	marioHP = currentHP;
}

Entity* QuestionBlock::SpawnItem() {
	Entity* item = nullptr;
	
	//extraData size:
	//2 -> Coin
	//4 -> HP: 2 -> Leaf else Mushroom

	if (extraData.size() == 2) {
		item = new Coin;
		item->SetObjectID(static_cast<int>(Entity::ObjectType::OBJECT_TYPE_COIN));
		item->ParseData(extraData.at(0), extraData.at(1), colorKey);
		item->SetPosition(D3DXVECTOR3(originalPos.x, originalPos.y - item->GetBoxHeight() - 1, 0));
		item->SetCurrenHitPoints(2);
		item->SetVelocity(D3DXVECTOR3(0, -0.18f, 0));
	}
	else if (extraData.size() > 2) {
		//Big Mario
		if (marioHP >= 2) {
			item = new SuperLeaf;
			item->SetObjectID(static_cast<int>(Entity::ObjectType::OBJECT_TYPE_LEAF));
			item->ParseData(extraData.at(2), extraData.at(3), colorKey);
			item->SetPosition(D3DXVECTOR3(originalPos.x, originalPos.y - item->GetBoxHeight() - 1, 0));
			item->SetVelocity(D3DXVECTOR3(0, -0.008f, 0));
		}
		else {
			item = new SuperMushroom;
			item->SetObjectID(static_cast<int>(Entity::ObjectType::OBJECT_TYPE_MUSHROOM));
			item->ParseData(extraData.at(0), extraData.at(1), colorKey);
			item->SetPosition(D3DXVECTOR3(originalPos.x, originalPos.y - item->GetBoxHeight() / 3, 0));
			dynamic_cast<SuperMushroom*>(item)->StartEmergeTimer();
		}
	}

	extraData.clear();
	return item;
}

void QuestionBlock::HandleStates() {
	switch (hitPoints) {
		case 1:
			currentState = BlockState::PUSHED;
			break;
		case 2:
			currentState = BlockState::ROTATE;
			break;
	}
}

void QuestionBlock::TakeDamage() {
	if (hitPoints > 1) {
		--hitPoints;
		
		if (position.y >= (originalPos.y - MAX_Y_OFFSET)) {
			velocity.y = -jumpSpeed;
		}

		//SceneManager::GetInstance()->GetCurrentScene()->AddObjectToScene(SpawnItem());
	}
}

void QuestionBlock::Update(DWORD delta, std::vector<GameObject*>* objects) {
	HandleStates();

	GameObject::Update(delta);
	
	if (position.y < originalPos.y) {
		velocity.y += gravity * delta;
	}
	else {
		velocity.y = 0;
		if (position.y >= originalPos.y) {
			position = originalPos;			
		}
		else if (position.y < (originalPos.y - MAX_Y_OFFSET)) {
			position.y = originalPos.y - MAX_Y_OFFSET;
		}
	}

	position += distance;
}

void QuestionBlock::Render() {
	switch (currentState) {
		case BlockState::PUSHED:
			sprite.PlayAnimation("None", position);
			break;
		case BlockState::ROTATE:
			sprite.PlayAnimation("Rotate", position);
			break;
	}
}

void QuestionBlock::Release() {
	
}
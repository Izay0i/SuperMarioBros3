#include "../../headers/tiles/ShinyBrick.h"

LPCWSTR ShinyBrick::texturePath = nullptr;
LPDIRECT3DTEXTURE9 ShinyBrick::texture = nullptr;
D3DCOLOR ShinyBrick::colorKey = D3DCOLOR_XRGB(0, 0, 0);

ShinyBrick::ShinyBrick() {
	//1 - no items
	//2 - rotate sprite
	//3 - switched to coin
	hitPoints = 2;
	score = 0;
}

RECTF ShinyBrick::GetBoundingBox(int id) const {
	RECTF bound;
	
	if (removeStart == 0) {
		bound.left = position.x + 2;
		bound.top = position.y + 1;
		bound.right = position.x + hitBox.GetWidth(id) - 3;
		bound.bottom = position.y + hitBox.GetHeight(id) - 5;
	}

	return bound;
}

void ShinyBrick::LoadTexture() {
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

void ShinyBrick::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void ShinyBrick::ParseHitboxes(std::string line) {
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

void ShinyBrick::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::vector<std::string> extraData) {
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

	amount = extraData.size() > 0 ? std::stoi(extraData.at(1)) : 0;
	readFile.close();
}

Entity* ShinyBrick::SpawnItem() {
	Entity::ObjectType objectID = static_cast<Entity::ObjectType>(std::stoi(extraData.at(0)));

	Entity* item = nullptr;

	switch (objectID) {
		case Entity::ObjectType::OBJECT_TYPE_1UPSHROOM:
			item = new GMushroom;
			item->SetObjectID(static_cast<int>(Entity::ObjectType::OBJECT_TYPE_1UPSHROOM));
			item->ParseData(extraData.at(2), extraData.at(3), colorKey);
			item->SetPosition(D3DXVECTOR3(originalPos.x, originalPos.y - item->GetBoxHeight() / 3, 0));
			dynamic_cast<GMushroom*>(item)->StartEmergeTimer();
			break;
		case Entity::ObjectType::OBJECT_TYPE_SWITCHBLOCK:
			item = new SwitchBlock;
			item->SetObjectID(static_cast<int>(Entity::ObjectType::OBJECT_TYPE_SWITCHBLOCK));
			item->ParseData(extraData.at(2), extraData.at(3), colorKey);
			item->SetPosition(D3DXVECTOR3(originalPos.x, originalPos.y - item->GetBoxHeight(), 0));
			break;
		case Entity::ObjectType::OBJECT_TYPE_COIN:
			item = new Coin;
			item->SetObjectID(static_cast<int>(Entity::ObjectType::OBJECT_TYPE_COIN));
			item->ParseData(extraData.at(2), extraData.at(3), colorKey);
			item->SetPosition(D3DXVECTOR3(originalPos.x, originalPos.y - item->GetBoxHeight() - 1, 0));
			item->SetCurrenHitPoints(2);
			item->SetVelocity(D3DXVECTOR3(0, -0.18f, 0));
			break;
	}

	--amount;

	if (amount == 0) {
		extraData.clear();
	}

	tookDamage = false;
	return item;
}

void ShinyBrick::HandleStates() {
	switch (hitPoints) {
		case 1:
			currentState = BlockState::PUSHED;
			break;
		case 2:
			currentState = BlockState::ROTATE;
			break;
		case 3:
			currentState = BlockState::SWITCHEDTOCOIN;
			break;
	}
}

void ShinyBrick::TakeDamage() {
	if (hitPoints > 1 && amount == 1) {
		--hitPoints;
	}

	if (amount > 0) {
		if (position.y >= (originalPos.y - MAX_Y_OFFSET)) {
			velocity.y = -jumpSpeed;
		}

		tookDamage = true;
	}
}

void ShinyBrick::Update(DWORD delta, std::vector<GameObject*>* objects) {
	HandleStates();

	GameObject::Update(delta);
	
	if (removeStart != 0 && GetTickCount64() - removeStart > removeTime) {
		hitPoints = -1;
		removeStart = 0;
	}

	if (currentState != BlockState::SWITCHEDTOCOIN) {
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

	std::vector<LPCOLLISIONEVENT> collisionEvents, eventResults;
	collisionEvents.clear();

	CalcPotentialCollision(objects, collisionEvents);

	D3DXVECTOR2 minTime;
	D3DXVECTOR2 offSet(0.4f, 0.4f);
	D3DXVECTOR3 normal;
	D3DXVECTOR3 relativeDistance;

	FilterCollision(collisionEvents, eventResults, minTime, normal, relativeDistance);

	for (LPCOLLISIONEVENT result : eventResults) {
		LPCOLLISIONEVENT event = result;

		//mario's fireball
		if (dynamic_cast<Entity*>(event->object) && event->object->GetObjectID() == 99) {
			if (hitPoints == 2 && amount == 0) {
				StartRemoveTimer();
			}
		}
	}

	for (LPCOLLISIONEVENT event : collisionEvents) {
		delete event;
	}
}

void ShinyBrick::Render() {
	if (IsBeingRemoved()) {
		sprite.PlayAnimation("Break", D3DXVECTOR3(position.x - 14, position.y - 13, 0));
	}
	else {
		switch (currentState) {
			case BlockState::PUSHED:
				sprite.PlayAnimation("None", position);
				break;
			case BlockState::ROTATE:
				sprite.PlayAnimation("Rotate", position);
				break;
			case BlockState::SWITCHEDTOCOIN:
				sprite.PlayAnimation("Coin", position);
				break;
		}
	}
}

void ShinyBrick::Release() {
	
}
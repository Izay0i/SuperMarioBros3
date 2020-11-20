#include "../../headers/npc/Fireball.h"

LPCWSTR Fireball::texturePath = nullptr;
LPDIRECT3DTEXTURE9 Fireball::texture = nullptr;
D3DCOLOR Fireball::colorKey = D3DCOLOR_XRGB(0, 0, 0);

Fireball::Fireball() {
	hitPoints = 1;
	currentState = BallState::BOUNCE;
}

void Fireball::LoadTexture() {
	if (!texture) {
		HRESULT hResult;
		D3DXIMAGE_INFO imageInfo;

		hResult = D3DXGetImageInfoFromFile(texturePath, &imageInfo);
		if (hResult != D3D_OK) {
			OutputDebugStringA("[MARIO] Failed to get image info\n");
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

RECTF Fireball::GetBoundingBox(int id) const {
	RECTF bound;
	bound.left = position.x;
	bound.top = position.y;
	bound.right = position.x + hitBox.GetWidth(id);
	bound.bottom = position.y + hitBox.GetHeight(id);
	return bound;
}

void Fireball::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void Fireball::ParseHitboxes(std::string line) {
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

void Fireball::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey) {
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

void Fireball::HandleStates() {
	switch (hitPoints) {
		case 0:
			currentState = BallState::EXPLODE;
			break;
		case 1:
			currentState = BallState::BOUNCE;
			break;
	}

	switch (currentState) {
		case BallState::BOUNCE:
			velocity.x = runSpeed * normal.x;
			break;
		case BallState::EXPLODE:
			velocity = D3DXVECTOR3(0, 9999, 0);
			break;
	}
}

void Fireball::TakeDamage() {
	if (hitPoints >= 1) {
		--hitPoints;
	}
}

void Fireball::Update(DWORD delta, std::vector<GameObject*>* objects) {
	HandleStates();

	GameObject::Update(delta);

	velocity.y += gravity * delta;

	std::vector<LPCOLLISIONEVENT> collisionEvents, eventResults;
	collisionEvents.clear();

	if (hitPoints != 0) {
		CalcPotentialCollision(objects, collisionEvents);
	}

	if (collisionEvents.size() == 0) {
		position += distance;
	}
	else {
		D3DXVECTOR2 minTime;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 relativeDistance;

		FilterCollision(collisionEvents, eventResults, minTime, normal, relativeDistance);

		position.x += minTime.x * distance.x + normal.x * 0.4f;
		position.y += minTime.y * distance.y + normal.y * 0.4f;

		if (normal.x != 0.0f) {
			velocity.x = 0.0f;
		}

		if (normal.y != 0.0f) {
			velocity.y = -jumpSpeed;
		}

		for (LPCOLLISIONEVENT result : eventResults) {
			LPCOLLISIONEVENT event = result;			

			//ignore one-way platforms
			if (event->object->GetObjectID() == 205) {
				continue;
			}

			if (dynamic_cast<Tiles*>(event->object)) {				
				if (event->normal.x != 0.0f) {
					TakeDamage();
				}				
			}
			
			if ((dynamic_cast<Entity*>(event->object))) {
				if (event->normal.x != 0.0f || event->normal.y != 0.0f) {
					dynamic_cast<Entity*>(event->object)->TakeDamage();
					TakeDamage();
				}
			}
		}
	}

	for (LPCOLLISIONEVENT event : collisionEvents) {
		delete event;
	}
}

void Fireball::Render() {
	switch (currentState) {
		case BallState::BOUNCE:
			sprite.PlayAnimation("Bounce", position, D3DXVECTOR2(normal.x, 1.0f));
			break;
		case BallState::EXPLODE:
			sprite.PlayAnimation("Explode", position);
			break;
	}
}

void Fireball::Release() {

}
#include "../../headers/npc/Goomba.h"

LPCWSTR Goomba::texturePath = nullptr;
LPDIRECT3DTEXTURE9 Goomba::texture = nullptr;
D3DCOLOR Goomba::colorKey = D3DCOLOR_XRGB(0, 0, 0);

Goomba::Goomba() {
	hitPoints = 1;
	currentState = GoombaState::WALK;
}

void Goomba::LoadTexture() {
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

RECTF Goomba::GetBoundingBox(int id) const {
	RECTF bound;
	bound.left = position.x + 1;
	bound.top = position.y + 1;
	bound.right = position.x + hitBox.GetWidth(id);

	if (hitPoints != 0) {
		bound.bottom = position.y + hitBox.GetHeight(id);
	}
	else {
		bound.bottom = position.y + hitBox.GetHeight(1);
	}

	return bound;
}

void Goomba::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void Goomba::ParseHitboxes(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 4) {
		return;
	}

	unsigned int left = atoi(tokens.at(0).c_str());
	unsigned int top = atoi(tokens.at(1).c_str());
	unsigned int right = atoi(tokens.at(2).c_str());
	unsigned int bottom = atoi(tokens.at(3).c_str());

	RECTF hitbox;
	hitbox.left = left;
	hitbox.top = top;
	hitbox.right = right;
	hitbox.bottom = bottom;

	this->hitBox.AddHitBox(hitbox);
}

void Goomba::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey) {
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

void Goomba::HandleStates() {
	if (hitPoints == 0) {
		currentState = GoombaState::DIE;
	}

	switch (currentState) {
		case GoombaState::WALK:
			velocity.x = -runSpeed * normal.x;
			break;
		case GoombaState::DIE:
			velocity = D3DXVECTOR3(0, 0, 0);
			break;
	}
}

void Goomba::TakeDamage() {
	if (hitPoints > 0) {
		--hitPoints;
	}
}

void Goomba::Update(DWORD delta, std::vector<GameObject*>* objects) {
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

		position.x += minTime.x * distance.x + normal.x * 0.1f;
		position.y += minTime.y * distance.y + normal.y * 0.1f;

		if (normal.x != 0.0f) {
			velocity.x = 0.0f;
		}

		if (normal.y != 0.0f) {
			velocity.y = 0.0f;
		}

		for (LPCOLLISIONEVENT result : eventResults) {
			LPCOLLISIONEVENT event = result;

			if (dynamic_cast<Entity*>(event->object) || dynamic_cast<Tiles*>(event->object)) {
				if (event->normal.x != 0.0f) {
					this->normal.x = -event->normal.x;
				}
			}
			
		}
	}

	for (LPCOLLISIONEVENT event : collisionEvents) {
		delete event;
	}
}

void Goomba::Render() {
	switch (currentState) {
		case GoombaState::WALK:
			sprite.PlayAnimation("Walk", position);
			break;
		case GoombaState::DIE:
			sprite.PlayAnimation("Die", position);
			break;
	}
}

void Goomba::Release() {

}
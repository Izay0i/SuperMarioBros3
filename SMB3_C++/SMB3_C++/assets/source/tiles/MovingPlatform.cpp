#include "../../headers/tiles/MovingPlatform.h"

LPCWSTR MovingPlatform::texturePath = nullptr;
LPDIRECT3DTEXTURE9 MovingPlatform::texture = nullptr;
D3DCOLOR MovingPlatform::colorKey = D3DCOLOR_XRGB(0, 0, 0);

MovingPlatform::MovingPlatform() {
	//0 - falling
	//1 - moving
	hitPoints = 1;
}

RECTF MovingPlatform::GetBoundingBox(int id) const {
	RECTF bound;
	bound.left = position.x + 2;
	bound.top = position.y + 2;
	bound.right = position.x + hitBox.GetWidth(id) - 3;
	bound.bottom = position.y + hitBox.GetHeight(id) - 5;

	return bound;
}

void MovingPlatform::LoadTexture() {
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

void MovingPlatform::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void MovingPlatform::ParseHitboxes(std::string line) {
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

void MovingPlatform::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::vector<std::string> extraData) {
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

void MovingPlatform::HandleStates() {
	switch (hitPoints) {
		case 0:
			velocity.x = 0;
			currentState = PlatformState::FALLING;
			break;
		case 1:
			velocity.x = -moveSpeed;
			currentState = PlatformState::MOVING;
			break;
	}
}

void MovingPlatform::TakeDamage() {
	if (hitPoints > 0) {
		--hitPoints;
	}
}

void MovingPlatform::Update(DWORD delta, std::vector<GameObject*>* objects) {
	HandleStates();
	
	GameObject::Update(delta);
	switch (currentState) {
		case PlatformState::FALLING:
			velocity.y += gravity * delta;
			break;
	}
	position += distance;
}

void MovingPlatform::Render() {
	sprite.PlayAnimation("Left", position);
	sprite.PlayAnimation("Middle", D3DXVECTOR3(position.x + 16, position.y, 0));
	sprite.PlayAnimation("Right", D3DXVECTOR3(position.x + 32, position.y, 0));
}

void MovingPlatform::Release() {

}
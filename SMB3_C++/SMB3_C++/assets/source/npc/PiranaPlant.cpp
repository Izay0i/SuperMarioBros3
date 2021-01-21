#include "../../headers/npc/PiranaPlant.h"

LPCWSTR PiranaPlant::texturePath = nullptr;
LPDIRECT3DTEXTURE9 PiranaPlant::texture = nullptr;
D3DCOLOR PiranaPlant::colorKey = D3DCOLOR_XRGB(0, 0, 0);

PiranaPlant::PiranaPlant() {
	//0 die
	//1 biting
	hitPoints = 2;
	normal = D3DXVECTOR3(1, 1, 0);
	StartCoolDownTimer();
}

void PiranaPlant::LoadTexture() {
	if (!texture) {
		HRESULT hResult;
		D3DXIMAGE_INFO imageInfo;

		hResult = D3DXGetImageInfoFromFile(texturePath, &imageInfo);
		if (hResult != D3D_OK) {
			OutputDebugStringA("[COIN] Failed to get image info\n");
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

RECTF PiranaPlant::GetBoundingBox(int id) const {
	RECTF bound;

	if (hitPoints > 0) {
		bound.left = position.x;
		bound.top = position.y;
		bound.right = position.x + hitBox.GetWidth(id);
		bound.bottom = position.y + hitBox.GetHeight(id);
	}

	return bound;
}

void PiranaPlant::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void PiranaPlant::ParseHitboxes(std::string line) {
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

void PiranaPlant::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::vector<std::string> extraData) {
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

void PiranaPlant::HandleStates() {
	switch (hitPoints) {
		case 0:
			currentState = PlantState::DIE;
			break;
		case 1:
			currentState = PlantState::BITING;
			break;
	}
}

void PiranaPlant::TakeDamage() {
	hitPoints = 0;
	StartRemoveTimer();
}

void PiranaPlant::Update(DWORD delta, std::vector<GameObject*>* objects) {
	HandleStates();

	/*char debug[100];
	sprintf_s(debug, "Posy: %f\n", position.y);
	OutputDebugStringA(debug);*/

	velocity.y = 0.02f * normal.y;
	GameObject::Update(delta);
	position += distance;

	if (IsOnCoolDown()) {
		//going down
		if (position.y < (originalPos.y - MAX_Y_OFFSET)) {
			if (!IsOnCoolDown()) {
				normal.y = 1.0f;
				StartCoolDownTimer();
			}
		}
		else if (position.y >= originalPos.y) {
			position.y = originalPos.y;
			if (!IsOnCoolDown()) {
				normal.y = -1.0f;
				StartCoolDownTimer();
			}
		}
	}
	

	if (coolDownStart != 0 && GetTickCount64() - coolDownStart > coolDownTime) {
		coolDownStart = 0;
	}

	if (removeStart != 0 && GetTickCount64() - removeStart > removeTime) {
		hitPoints = -1;
		removeStart = 0;
	}
}

void PiranaPlant::Render() {
	switch (currentState) {
		case PlantState::BITING:
			sprite.PlayAnimation("Bite", position);
			break;
		case PlantState::DIE:
			sprite.PlayAnimation("Die", position);
			sprite.PlayAnimation("100", position);
			break;
	}
}

void PiranaPlant::Release() {

}
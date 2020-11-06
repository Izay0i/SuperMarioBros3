#include "../../headers/player/Mario.h"

Entity* Mario::marioInstance = nullptr;

Mario::Mario() {
	sprite = new AnimatedSprite;

	currentForm = MarioForm::BIG;
	currentState = MarioState::IDLE;

	scale = D3DXVECTOR2(1.0f, 1.0f);
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

void Mario::SetMarioForm(MarioForm form) {
	currentForm = form;
}

Mario::MarioForm Mario::GetMarioForm() {
	return currentForm;
}

void Mario::SetState(MarioState state) {
	currentState = state;
}

Mario::MarioState Mario::GetState() {
	return currentState;
}

void Mario::CheckCollision(Entity* movingEntity, Entity* staticEntity) {

}

void Mario::HandleStates() {
	switch (currentState) {
		case MarioState::IDLE:
			if (velocity.x != 0.0f) {
				SetState(MarioState::RUN);
			}
			else if (velocity.y < 0.0f) {
				SetState(MarioState::JUMP);
			}
			else if (velocity.y > 0.0f) {
				SetState(MarioState::FALL);
			}
			break;
		case MarioState::RUN:
			if (velocity.x == 0.0f) {
				SetState(MarioState::IDLE);
			}
			break;
		case MarioState::TURN:

			break;
		case MarioState::JUMP:
			if (velocity.y > 0.0f) {
				//SetStates(MarioState::FALL);
			}
			break;
		case MarioState::FALL:

			break;
		case MarioState::HOLD:

			break;
		case MarioState::KICK:

			break;
		case MarioState::DIE:

			break;
		case MarioState::CROUCH:
			if (velocity.x != 0.0f) {
				SetState(MarioState::RUN);
			}
			else if (velocity.y < 0.0f) {
				SetState(MarioState::JUMP);
			}
			break;
	}
}

void Mario::OnKeyDown(int keyCode) {
	switch (keyCode) {
		case VK_A:
			velocity.x = -runSpeed;
			//to flip the sprite
			scale = D3DXVECTOR2(1.0f, 1.0f);
			break;
		case VK_D:
			velocity.x = runSpeed;
			scale = D3DXVECTOR2(-1.0f, 1.0f);
			break;
		case VK_S:
			SetState(MarioState::CROUCH);
			velocity.y = jumpSpeed;
			//scale = D3DXVECTOR2(1.0f, 1.0f);
			break;
		case VK_W:
			scale = D3DXVECTOR2(1.0f, 1.0f);
			break;
		case VK_SPACE:
			velocity.y = -jumpSpeed;
			//scale = D3DXVECTOR2(1.0f, 1.0f);
			break;
		case VK_K: //fire or spin
			velocity.x = 0;
			velocity.y = 0;
			scale = D3DXVECTOR2(-1.0f, 1.0f);
			break;
	}
}

void Mario::OnKeyUp(int keyCode) {	
	
}

void Mario::Update(DWORD delta) {
	HandleStates();

	distance = velocity * delta;
	position += distance;
}

void Mario::Render() {
	switch (GetState()) {
		case MarioState::IDLE:
			switch (GetMarioForm()) {
				case MarioForm::SMALL:
					sprite->PlayAnimation("Idle", position, scale);
					break;
				case MarioForm::BIG:
					sprite->PlayAnimation("BigIdle", position, scale);
					break;
				case MarioForm::FIRE:
					sprite->PlayAnimation("FireIdle", position, scale);
					break;
				case MarioForm::RACOON:
					sprite->PlayAnimation("RacIdle", position, scale);
					break;
			}
			break;
		case MarioState::RUN:
			switch (GetMarioForm()) {
				case MarioForm::SMALL:
					sprite->PlayAnimation("Run", position, scale);
					break;
				case MarioForm::BIG:
					sprite->PlayAnimation("BigRun", position, scale);
					break;
				case MarioForm::FIRE:
					sprite->PlayAnimation("FireRun", position, scale);
					break;
				case MarioForm::RACOON:
					sprite->PlayAnimation("RacRun", position, scale);
					break;
			}
			break;
		case MarioState::JUMP:
			switch (GetMarioForm()) {
				case MarioForm::SMALL:
					sprite->PlayAnimation("Jump", position, scale);
					break;
				case MarioForm::BIG:
					sprite->PlayAnimation("BigJump", position, scale);
					break;
				case MarioForm::FIRE:
					sprite->PlayAnimation("FireJump", position, scale);
					break;
				case MarioForm::RACOON:
					sprite->PlayAnimation("RacJump", position, scale);
					break;
			}
			break;
		case MarioState::CROUCH:
			switch (GetMarioForm()) {
				case MarioForm::BIG:
					sprite->PlayAnimation("BigCrouch", position, scale);
					break;
				case MarioForm::FIRE:
					sprite->PlayAnimation("FireCrouch", position, scale);
					break;
				case MarioForm::RACOON:
					sprite->PlayAnimation("RacCrouch", position, scale);
					break;
				}
			break;
		case MarioState::SHOOT:
			switch (GetMarioForm()) {
				case MarioForm::FIRE:
					sprite->PlayAnimation("FireShoot", position, scale);
					break;
			}
			break;
		case MarioState::SPIN:
			switch (GetMarioForm()) {
			case MarioForm::RACOON:
				sprite->PlayAnimation("RacSpin", position, scale);
				break;
			}
			break;
		default:
			SetState(MarioState::IDLE);
	}
}

void Mario::Release() {
	if (marioInstance) {
		delete marioInstance;
		marioInstance = nullptr;
	}
	
	if (sprite) {
		delete sprite;
		sprite = nullptr;
	}
}
#include "../../headers/player/Mario.h"

Entity* Mario::marioInstance = nullptr;

Mario::Mario() {
	sprite = new AnimatedSprite;
	currentForm = MarioForm::RACOON;
	currentState = MarioState::IDLE;
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

void Mario::OnKeyDown(int keyCode) {
	switch (keyCode) {
		case VK_A:
			SetState(MarioState::RUN);
			acceleration = -1;
			//OutputDebugStringA("A key pressed\n");
			break;
		case VK_D:
			SetState(MarioState::RUN);
			acceleration = 1;
			//OutputDebugStringA("D key pressed\n");
			break;
		case VK_S:
			SetState(MarioState::CROUCH);
			//OutputDebugStringA("S key pressed\n");
			break;
		case VK_W:
			//OutputDebugStringA("W key pressed\n");
			break;
		case VK_SPACE:
			SetState(MarioState::JUMP);
			//OutputDebugStringA("Space key pressed\n");
			break;
		case VK_F: //fire or spin
			acceleration = 0;
			velocity.x = 0;
			velocity.y = 0;
			switch (GetMarioForm()) {
				case MarioForm::FIRE:
					SetState(MarioState::SHOOT);
					break;
				case MarioForm::RACOON:
					SetState(MarioState::SPIN);
					break;
			}
			//OutputDebugStringA("F key pressed\n");
			break;
	}
}

void Mario::OnKeyUp(int keyCode) {	
	
}

void Mario::Update(DWORD delta) {
	switch (GetState()) {
		case MarioState::RUN:
			velocity.x = runSpeed * acceleration;
			break;
		case MarioState::JUMP:
			velocity.y = -jumpSpeed;
			break;
		case MarioState::CROUCH:
			velocity.y = jumpSpeed;
			break;
	}

	distance = velocity * delta;
	position += distance;

	/*char debugStr[100];
	sprintf_s(debugStr, "Velocity: %f %f\tPosition: %f %f\n", velocity.x, velocity.y, position.x, position.y);
	OutputDebugStringA(debugStr);*/
}

void Mario::Render() {
	switch (GetState()) {
		case MarioState::IDLE:
			switch (GetMarioForm()) {
				case MarioForm::SMALL:
					sprite->PlayAnimation("Idle", position);
					break;
				case MarioForm::BIG:
					sprite->PlayAnimation("BigIdle", position);
					break;
				case MarioForm::FIRE:
					sprite->PlayAnimation("FireIdle", position);
					break;
				case MarioForm::RACOON:
					sprite->PlayAnimation("RacIdle", position);
					break;
			}
			break;
		case MarioState::RUN:
			switch (GetMarioForm()) {
				case MarioForm::SMALL:
					sprite->PlayAnimation("Run", position);
					break;
				case MarioForm::BIG:
					sprite->PlayAnimation("BigRun", position);
					break;
				case MarioForm::FIRE:
					sprite->PlayAnimation("FireRun", position);
					break;
				case MarioForm::RACOON:
					sprite->PlayAnimation("RacRun", position);
					break;
			}
			break;
		case MarioState::JUMP:
			switch (GetMarioForm()) {
				case MarioForm::SMALL:
					sprite->PlayAnimation("Jump", position);
					break;
				case MarioForm::BIG:
					sprite->PlayAnimation("BigJump", position);
					break;
				case MarioForm::FIRE:
					sprite->PlayAnimation("FireJump", position);
					break;
				case MarioForm::RACOON:
					sprite->PlayAnimation("RacJump", position);
					break;
			}
			break;
		case MarioState::CROUCH:
			switch (GetMarioForm()) {
				case MarioForm::BIG:
					sprite->PlayAnimation("BigCrouch", position);
					break;
				case MarioForm::FIRE:
					sprite->PlayAnimation("FireCrouch", position);
					break;
				case MarioForm::RACOON:
					sprite->PlayAnimation("RacCrouch", position);
					break;
				}
			break;
		case MarioState::SHOOT:
			switch (GetMarioForm()) {
				case MarioForm::FIRE:
					sprite->PlayAnimation("FireShoot", position);
					break;
			}
			break;
		case MarioState::SPIN:
			switch (GetMarioForm()) {
			case MarioForm::RACOON:
				sprite->PlayAnimation("RacSpin", position);
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
}
#include "Mario.h"

LPDIRECT3DTEXTURE9 Mario::_playerTexture = nullptr;

void Mario::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _playerTexture);
}

Mario::Mario() {
	_scale = D3DXVECTOR2(-1.0f, 1.0f);

	_runSpeed = 0.09f;
	_jumpSpeed = 0.327f;
	_bounceSpeed = 0.4f;
	_gravity = 0.0025f;
	_acceleration = 0.5f;

	_heldEntity = nullptr;
	_touchedEntity = nullptr;
}

Mario::~Mario() {}

RECTF Mario::GetBoundingBox(int index) const {
	return GameObject::GetBoundingBox(index);
}

void Mario::HandleStates() {
	//_stateMachine = new State;
}

void Mario::OnKeyUp(int keyCode) {
	Device::KeyInput keyInput = static_cast<Device::KeyInput>(keyCode);
	switch (keyInput) {
		case Device::KeyInput::KEY_INPUT_UP:

			break;
		case Device::KeyInput::KEY_INPUT_DOWN:

			break;
		case Device::KeyInput::KEY_INPUT_LEFT:

			break;
		case Device::KeyInput::KEY_INPUT_RIGHT:

			break;
		case Device::KeyInput::KEY_INPUT_B:

			break;
		case Device::KeyInput::KEY_INPUT_A:

			break;
	}
}

void Mario::OnKeyDown(int keyCode) {
	Device::KeyInput keyInput = static_cast<Device::KeyInput>(keyCode);
	switch (keyInput) {
		case Device::KeyInput::KEY_INPUT_UP:

			break;
		case Device::KeyInput::KEY_INPUT_DOWN:

			break;
		case Device::KeyInput::KEY_INPUT_LEFT:

			break;
		case Device::KeyInput::KEY_INPUT_RIGHT:

			break;
		case Device::KeyInput::KEY_INPUT_B:

			break;
		case Device::KeyInput::KEY_INPUT_A:

			break;
	}
}

void Mario::ParseData(
	std::string dataPath, 
	const LPDIRECT3DTEXTURE9& texture, 
	std::vector<std::string> extraData) 
{
	Entity::ParseData(dataPath, texture, extraData);
	_playerTexture = texture;
}

void Mario::MoveLeft() {

}

void Mario::MoveRight() {

}

void Mario::Jump() {

}

void Mario::Crouch() {

}

void Mario::HoldEntity() {

}

void Mario::Update(DWORD delta, std::vector<GameObject*>* objects) {

}

void Mario::Render() {
	//_stateMachine->Render();
}

void Mario::Release() {
	/*
	for (auto& fireball : _fireballs) {
		fireball->Release();
		delete fireball;
	}
	_fireballs.clear();
	*/

	_animatedSprite.Release();
}
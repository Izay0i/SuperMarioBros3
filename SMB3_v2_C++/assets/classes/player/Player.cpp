#include "Player.h"

LPDIRECT3DTEXTURE9 Player::_playerTexture = nullptr;

void Player::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _playerTexture);
}

Player::Player() {
	_scale = D3DXVECTOR2(-1.0f, 1.0f);
	_renderPriority = 0;

	_runSpeed = 0.09f;
	_jumpSpeed = 0.327f;
	_bounceSpeed = 0.4f;
	_gravity = 0.0025f;
	_acceleration = 0.5f;

	_stateMachine = nullptr;

	_heldEntity = nullptr;
	_touchedEntity = nullptr;
}

Player::~Player() {}

RECTF Player::GetBoundingBox(int index) const {
	return GameObject::GetBoundingBox(index);
}

Entity* Player::GetHeldEntity() const {
	return _heldEntity;
}

bool Player::IsOnGround() const {
	return _isOnGround;
}

void Player::HandleStates(int keyCode, bool isKeyDown) {
	_stateMachine->HandleStates(keyCode, isKeyDown);

	GlobalUtil::KeyInput keyInput = static_cast<GlobalUtil::KeyInput>(keyCode);
	if (isKeyDown) {
		switch (keyInput) {
			case GlobalUtil::KeyInput::KEY_INPUT_1:
				
				break;
		}
	}
	else {
		switch (keyInput) {
			case GlobalUtil::KeyInput::KEY_INPUT_2:
				
				break;
		}
	}
}

void Player::OnKeyUp(int keyCode) {
	GlobalUtil::KeyInput keyInput = static_cast<GlobalUtil::KeyInput>(keyCode);
	switch (keyInput) {
		case GlobalUtil::KeyInput::KEY_INPUT_LEFT:
		case GlobalUtil::KeyInput::KEY_INPUT_RIGHT:
			//Slippery feel when the player stops
			if (_acceleration <= 0.5f) {
				_velocity.x = 0.0f;
				_acceleration = 0.5f;
			}

			if (_acceleration > 0.5f) {
				_acceleration -= 0.06f;
			}
			break;
		case GlobalUtil::KeyInput::KEY_INPUT_DOWN:

			break;
		case GlobalUtil::KeyInput::KEY_INPUT_A:
			if (_gravity < 0.0025f) {
				_gravity += 0.0005f;
			}
			break;
	}
}

void Player::OnKeyDown(int keyCode) {
	GlobalUtil::KeyInput keyInput = static_cast<GlobalUtil::KeyInput>(keyCode);
	switch (keyInput) {
		case GlobalUtil::KeyInput::KEY_INPUT_1:
			_health = 1;
			break;
		case GlobalUtil::KeyInput::KEY_INPUT_2:
			_health = 2;
			break;
		case GlobalUtil::KeyInput::KEY_INPUT_3:
			_health = 3;
			break;
		case GlobalUtil::KeyInput::KEY_INPUT_4:
			_health = 4;
			break;

		case GlobalUtil::KeyInput::KEY_INPUT_LEFT:
			MoveLeft();
			SkidLeft();
			break;
		case GlobalUtil::KeyInput::KEY_INPUT_RIGHT:
			MoveRight();
			SkidRight();
			break;
		case GlobalUtil::KeyInput::KEY_INPUT_B:

			break;
		case GlobalUtil::KeyInput::KEY_INPUT_A:
			Jump();
			break;
	}
}

void Player::ParseData(
	std::string dataPath, 
	const LPDIRECT3DTEXTURE9& texture, 
	std::vector<std::string> extraData) 
{
	if (_playerTexture == nullptr) {
		_playerTexture = texture;
	}
	_stateMachine = new StateMachine(this);
	Entity::ParseData(dataPath, texture, extraData);
}

void Player::MoveLeft() {
	_scale = D3DXVECTOR2(1.0f, 1.0f);
	_velocity.x = -_runSpeed * _acceleration;
}

void Player::MoveRight() {
	_scale = D3DXVECTOR2(-1.0f, 1.0f);
	_velocity.x = _runSpeed * _acceleration;
}

void Player::SkidLeft() {
	if (_acceleration < _ACCEL_THRESHOLD && _velocity.x != 0.0f) {
		if (_normal.x == 1.0f) {
			_acceleration = 0.0499f;
		}
	}
}

void Player::SkidRight() {
	if (_acceleration < _ACCEL_THRESHOLD && _velocity.x != 0.0f) {
		if (_normal.x == -1.0f) {
			_acceleration = 0.0499f;
		}
	}
}

void Player::Jump() {
	if (_isOnGround) {
		_velocity.y = -_jumpSpeed;
		_isOnGround = false;
	}
	
	//Variable jump height
	if (_gravity > _MAX_GRAVITY) {
		_gravity -= 0.0005f;
	}
	else if (_gravity <= _MAX_GRAVITY) {
		_gravity = _MAX_GRAVITY;
	}
}

void Player::Crouch() {
	
}

void Player::HoldEntity() {

}

void Player::HandleCollisionEventResults(
	LPCOLLISIONEVENT result, 
	D3DXVECTOR2& minTime, 
	D3DXVECTOR2& offset, 
	D3DXVECTOR2& normal, 
	D3DXVECTOR2& relativeDistance) 
{
	Entity* eventEntity = result->entity;
	D3DXVECTOR2 eventNormal = result->normal;

	if (eventEntity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_TILE) {

	}

	if (eventEntity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_MOVINGPLATFORM) {
		/*
		MovingPlatform* movingPlatform = eventEntity;
		if (event->normal.y == -1.0f) {
			movingPlatform->TakeDamage();
		}
		*/
	}
}

void Player::Update(DWORD deltaTime, std::vector<Entity*>* entities) {
	if (_isOnGround == true) {
		_gravity = 0.0025f;
	}
	
	_stateMachine->Update(deltaTime);

	Entity::Update(deltaTime);
}

void Player::Render() {
	_stateMachine->Render();
}

void Player::Release() {
	/*
	for (auto& fireball : _fireballs) {
		fireball->Release();
		delete fireball;
	}
	_fireballs.clear();
	*/

	if (_stateMachine != nullptr) {
		_stateMachine->Release();
		delete _stateMachine;
	}

	_animatedSprite.Release();
}
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

	_flyTime = 6000;
	_inPipeTime = 4000;
	_attackTime = 126;
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
	
}

void Player::OnKeyUp(int keyCode) {
	GlobalUtil::KeyInput keyInput = static_cast<GlobalUtil::KeyInput>(keyCode);
	switch (keyInput) {
		case GlobalUtil::KeyInput::KEY_INPUT_DOWN:
			/*
			if (_isOnGround && !IsInPipe() && _health > 1) {
				_isOnGround = false;
				_velocity.y = -0.3f;
			}
			*/
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
	_normal.x = -1.0f;
	_scale = D3DXVECTOR2(1.0f, 1.0f);
	_velocity.x = -_runSpeed * _acceleration;
}

void Player::MoveRight() {
	_normal.x = 1.0f;
	_scale = D3DXVECTOR2(-1.0f, 1.0f);
	_velocity.x = _runSpeed * _acceleration;
}

void Player::MoveFriction() {
	//Slippery feel when the player stops
	if (_acceleration <= 0.5f) {
		_velocity.x = 0.0f;
		_acceleration = 0.5f;
	}
	else if (_acceleration > 0.5f) {
		_acceleration -= 0.06f;
	}
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

void Player::RunFly() {
	if (_health == 4) {
		if (_acceleration >= _ACCEL_THRESHOLD /*|| IsFlying()*/) {
			/*
			if (_isOnGround && !IsFlying()) {
				_isOnGround = false;
				StartFlyTimer();
			}
			*/
			_velocity.y = -_jumpSpeed * 0.8f;
		}
	}
}

void Player::Fall() {
	if (_gravity < 0.0025f) {
		_gravity += 0.0005f;
	}
}

void Player::SlowFall() {
	if (_health == 4 && !_isOnGround) {
		_velocity.y *= 0.2f;
	}
}

void Player::HandleCollisionResult(
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

void Player::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (_isOnGround == true) {
		_gravity = 0.0025f;
	}
	
	_stateMachine->Update(deltaTime);
	
	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);

	if (_heldEntity != nullptr) {
		if (_heldEntity->GetHealth() == 0 || _heldEntity->GetHealth() == 3) {
			//_isHolding = false;

			if (_heldEntity->GetHealth() == 3) {
				_heldEntity->SetPosition(
					{ 
					_position.x + 17.0f * _normal.x, 
					_position.y - 14.0f 
					}
				);
			}

			//_heldEntity->isBeingHeld = false;
			_heldEntity = nullptr;
			return;
		}

		/*
		if (_isHolding) {
			D3DXVECTOR2 offset;
			offset.x = IsInPipe() ? 0.0f : 10.0f;
			offset.y = _health == 1 ? 11.0f : 2.0f;

			_heldEntity->SetPosition({ _position.x + offset.x * normal.x, _position.y - offset.y });
		}
		else {
			_isNextToShell = true;

			_heldEntity->TakeDamage();
			_heldEntity->SetNormal({ -normal.x, 0.0f });
			_heldEntity->isBeingHeld = false;
			_heldEntity = nullptr;
		}
		*/
	}
}

void Player::Render() {
	if (!_isActive) {
		return;
	}

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
	_playerTexture = nullptr;
}
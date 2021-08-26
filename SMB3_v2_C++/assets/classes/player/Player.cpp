#include "../Device.h"
#include "../SceneManager.h"
#include "Player.h"
#include "state/IdleState.h"
#include "state/RunState.h"
#include "state/JumpState.h"
#include "state/FallState.h"
#include "state/CrouchState.h"
#include "state/ThrowState.h"
#include "state/WagState.h"

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

	_lives = 3;
	_coins = 0;
	_score = 0;

	_heldEntity = nullptr;
	_touchedEntity = nullptr;

	_flyTime = 6000;
	_inPipeTime = 4000;
	_attackTime = 126;

	_playerState = new IdleState(this);

	_tail = new Tail(this);
	//SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(_tail);
}

Player::~Player() {}

RECTF Player::GetBoundingBox(int index) const {
	return GameObject::GetBoundingBox(_health >= 2);
}

Entity* Player::GetHeldEntity() const {
	return _heldEntity;
}

bool Player::TriggeredStageEnd() const {
	return _triggeredStageEnd;
}

bool Player::WentIntoPipe() const {
	return _wentIntoPipe;
}

bool Player::IsFlying() const {
	return _flyStart != 0;
}

bool Player::IsInPipe() const {
	return _inPipeStart != 0;
}

bool Player::IsAttacking() const {
	return _attackStart != 0;
}

void Player::HandleStates() {
	if (_isOnGround) {
		_gravity = 0.0025f;
	}
	
	//Variable jump height	
	if (Device::IsKeyDown(DIK_K)) {
		if (_gravity > _MAX_GRAVITY) {
			_gravity -= 0.0005f;
		}
		else if (_gravity <= _MAX_GRAVITY) {
			_gravity = _MAX_GRAVITY;
		}
	}
	else {
		if (_gravity < 0.0025f) {
			_gravity += 0.0005f;
		}
	}

	//Skid
	if (_acceleration < _ACCEL_THRESHOLD && _velocity.x != 0.0f) {
		if (_normal.x == -1) {
			if (Device::IsKeyDown(DIK_D)) {
				_acceleration = 0.0499f;
			}
		}
		else if (_normal.x == 1) {
			if (Device::IsKeyDown(DIK_A)) {
				_acceleration = 0.0499f;
			}
		}
	}

	if (Device::IsKeyDown(DIK_A)) {
		MoveLeft();
	}
	else if (Device::IsKeyDown(DIK_D)) {
		MoveRight();
	}
	else {
		//Slippery feel when the player stops
		if (_acceleration <= 0.5f) {
			_velocity.x = 0.0f;
			_acceleration = 0.5f;
		}
		else if (_acceleration > 0.5f) {
			_acceleration -= 0.06f;
		}
	}

	if (Device::IsKeyDown(DIK_A) || Device::IsKeyDown(DIK_D)) {
		//GOTTA GO FAAAST
		if (Device::IsKeyDown(DIK_J) && !IsFlying()) {
			if (_acceleration < _MAX_ACCEL) {
				_acceleration += 0.03f;
			}
		}
		else {
			if (_acceleration < _MIN_ACCEL) {
				_acceleration += 0.03f;
			}
			else if (_acceleration > _MIN_ACCEL) {
				_acceleration -= 0.02f;
			}
		}
	}

	//Float a bit longer when flying
	if (_acceleration >= _ACCEL_THRESHOLD) {
		_gravity = 0.0013f;
	}

	PlayerState* currentState = _playerState->HandleStates();
	if (currentState != nullptr) {
		delete _playerState;
		_playerState = currentState;
	}
}

void Player::OnKeyUp(int keyCode) {
	switch (keyCode) {
		case DIK_S:
			_isCrouching = false;
			
			if (_isOnGround && !IsInPipe() && _health > 1) {
				_isOnGround = false;
				_velocity.y = -0.3f;
			}
			break;
		case DIK_J:
			_isHolding = false;
			break;
	}
}

void Player::OnKeyDown(int keyCode) {
	switch (keyCode) {
		case DIK_1:
			_health = 1;
			break;
		case DIK_2:
			_health = 2;
			break;
		case DIK_3:
			_health = 3;
			break;
		case DIK_4:
			_health = 4;
			break;

		case DIK_A:
			_normal.x = -1.0f;
			break;
		case DIK_D:
			_normal.x = 1.0f;
			break;
		case DIK_S:
			_isCrouching = true;
			break;
		case DIK_J:
			_isHolding = true;
			//Tail attack
			if (_health == 4 && !IsAttacking()) {
				//StartAttackTimer();
				_velocity.x = 0.0f;
			}
			break;
		case DIK_K:
			SlowFall();
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

void Player::Jump() {
	if (_isOnGround) {
		_velocity.y = -_jumpSpeed;
		_isOnGround = false;
	}
}

void Player::RunFly() {
	if (_health == 4) {
		if (_acceleration >= _ACCEL_THRESHOLD || IsFlying()) {
			if (_isOnGround && !IsFlying()) {
				_isOnGround = false;
				//StartFlyTimer();
			}
			
			_velocity.y = -_jumpSpeed * 0.8f;
		}
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

	if (eventNormal.y == -1.0f) {
		_isOnGround = true;
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
	//To show the whole kicking animation
	if (_isNextToShell && GetTickCount64() % 500 == 0) {
		_isNextToShell = false;
	}

	//--------------------------------------
	//TIMERS
	//--------------------------------------
	if (_flyStart != 0 && (GetTickCount64() - _flyStart > _flyTime || _health != 4)) {
		_flyStart = 0;
	}

	if (_inPipeStart != 0 && GetTickCount64() - _inPipeStart > _inPipeTime) {
		_inPipeStart = 0;
	}

	if (_attackStart != 0 && GetTickCount64() - _attackStart > _attackTime) {
		_attackStart = 0;
	}
	//--------------------------------------
	//TIMERS
	//--------------------------------------

	if (_triggeredStageEnd) {
		MoveRight();
	}

	_playerState->Update(deltaTime);
	_tail->Update(deltaTime);
	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);

	if (_heldEntity != nullptr) {
		if (_heldEntity->GetHealth() == 0 || _heldEntity->GetHealth() == 3) {
			_isHolding = false;

			if (_heldEntity->GetHealth() == 3) {
				_heldEntity->SetPosition(
					{ 
					_position.x + 17.0f * _normal.x, 
					_position.y - 14.0f 
					}
				);
			}

			_heldEntity->isBeingHeld = false;
			_heldEntity = nullptr;
			return;
		}
	
		if (_isHolding) {
			D3DXVECTOR2 offset;
			offset.x = IsInPipe() ? 0.0f : 10.0f;
			offset.y = _health == 1 ? 11.0f : 2.0f;

			_heldEntity->SetPosition({ _position.x + offset.x * _normal.x, _position.y - offset.y });
		}
		else {
			_isNextToShell = true;

			_heldEntity->TakeDamage();
			_heldEntity->SetNormal({ -_normal.x, 0.0f });
			_heldEntity->isBeingHeld = false;
			_heldEntity = nullptr;
		}
	}
}

void Player::Render() {
	if (!_isActive) {
		return;
	}

	_playerState->Render();
}

void Player::Release() {
	/*
	for (auto& fireball : _fireballs) {
		fireball->Release();
		delete fireball;
	}
	_fireballs.clear();
	*/

	if (_playerState != nullptr) {
		_playerState->Release();
		delete _playerState;
	}

	if (_tail != nullptr) {
		_tail->Release();
		delete _tail;
	}

	_animatedSprite.Release();
	_playerTexture = nullptr;
}
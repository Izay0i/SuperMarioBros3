#include "../Device.h"
#include "../SceneManager.h"
#include "Player.h"
#include "state/IdleState.h"
#include "../EntityList.h"

Texture* Player::_playerTexture = nullptr;

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

	_health = 4;

	_lives = 3;
	_coins = 0;
	_score = 0;

	_fireballsCount = 0;

	_heldEntity = nullptr;
	_touchedEntity = nullptr;

	_flyTime = 6000;
	_inPipeTime = 4000;
	_attackTime = 300;
	_fireballCoolDownTime = 2500;
	_invulnerableTime = 3000;

	_playerState = new IdleState(this);
	_tail = new Tail(this);

	_bonusItems.reserve(3);
}

Player::~Player() {}

RECTF Player::GetBoundingBox(int index) const {
	return GameObject::GetBoundingBox(_health >= 2 && !_isInMap && !_isCrouching);
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

bool Player::IsOnFireballCoolDown() const {
	return _fireballCoolDownStart != 0;
}

bool Player::IsInvulnerable() const {
	return _invulnerableStart != 0;
}

void Player::StartFlyTimer() {
	_flyStart = static_cast<DWORD>(GetTickCount64());
}

void Player::StartInPipeTimer() {
	_inPipeStart = static_cast<DWORD>(GetTickCount64());
}

void Player::StartAttackTimer() {
	_attackStart = static_cast<DWORD>(GetTickCount64());
}

void Player::StartFireballCoolDownTimer() {
	_fireballCoolDownStart = static_cast<DWORD>(GetTickCount64());
}

void Player::StartInvulnerableTimer() {
	_invulnerableStart = static_cast<DWORD>(GetTickCount64());
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

			if (_health > 1 && _isOnGround && !IsInPipe()) {
				_isOnGround = false;
				_position.y -= GetBoxHeight(1);
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
			//Fireball attack
			if (_health == 3 && !_isCrouching) {
				if (_fireballsCount < _FIREBALLS_LIMIT) {
					SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(SpawnFireball());
					++_fireballsCount;

					if (_fireballsCount == _FIREBALLS_LIMIT) {
						StartFireballCoolDownTimer();
					}
				}
			}

			//Tail attack
			if (_health == 4 && !IsAttacking()) {
				StartAttackTimer();
				SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(_tail);
			}
			break;
		case DIK_K:
			SlowFall();
			RunFly();
			Jump();
			break;
	}
}

void Player::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_playerTexture == nullptr) {
		_playerTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void Player::TakeDamage() {
	if (!IsInvulnerable()) {
		StartInvulnerableTimer();

		if (_health > 2) {
			_health = 2;
		}
		else {
			--_health;
		}
	}
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
				StartFlyTimer();
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

Fireball* Player::SpawnFireball() {
	Fireball* fireball = dynamic_cast<Fireball*>(
		SceneManager::GetInstance()->GetCurrentScene()->CreateEntityFromData(
			_extraData.at(0),
			_extraData.at(1),
			_extraData.at(2)
		)
	);
	fireball->SetNormal({ _normal.x, fireball->GetNormal().y });
	fireball->SetPosition({ _position.x, _position.y + 10.0f });
	return fireball;
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

	if (eventEntity == nullptr) {
		return;
	}

	if (eventNormal.y == -1.0f) {
		_isOnGround = true;
	}

	switch (eventEntity->GetObjectType()) {
	//----------------------------------------------------------------------------
	//NPCs
	//----------------------------------------------------------------------------
		case GameObjectType::GAMEOBJECT_TYPE_GOOMBA:
		case GameObjectType::GAMEOBJECT_TYPE_PARAGOOMBA:
			{
				Goomba* goomba = dynamic_cast<Goomba*>(eventEntity);
				if (eventNormal.y == -1.0f) {
					if (goomba->GetHealth() > 0) {
						goomba->TakeDamage();
						_velocity.y = -_bounceSpeed;
					}
				}
				else if (eventNormal.y == 1.0f || eventNormal.x != 0.0f) {
					if (goomba->GetHealth() > 0) {
						TakeDamage();
						_velocity.y = -_bounceSpeed;
					}
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_KOOPA:
		case GameObjectType::GAMEOBJECT_TYPE_PARAKOOPA:
			{
				Koopa* koopa = dynamic_cast<Koopa*>(eventEntity);
				if (eventNormal.y == -1.0f) {
					if (koopa->GetHealth() > 0) {
						_velocity.y = -_bounceSpeed;
					}

					if (koopa->GetHealth() != 1) {
						koopa->TakeDamage();
					}
					else if (koopa->GetHealth() == 1) {
						koopa->SetHealth(2);
					}
				}
				else if (eventNormal.y == 1.0f) {
					if (koopa->GetHealth() > 1 && koopa->GetHealth() != 2) {
						TakeDamage();
						_velocity.y = -_bounceSpeed;
					}
					else if (koopa->GetHealth() == 2) {
						koopa->TakeDamage();
					}
				}
				else if (eventNormal.x != 0.0f) {
					if (koopa->GetHealth() == 2) {
						if (_isHolding) {
							if (_heldEntity == nullptr) {
								_heldEntity = koopa;
								koopa->isBeingHeld = true;
							}
						}
						else {
							_isNextToShell = true;
							koopa->TakeDamage();
							koopa->SetNormal({ -_normal.x, koopa->GetNormal().y });
						}
					}
					else {
						TakeDamage();
						_velocity.y = -_bounceSpeed;
					}
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_PIRAPLANT:
		case GameObjectType::GAMEOBJECT_TYPE_VENUSPLANT:
			{
				PiranaPlant* piranaPlant = dynamic_cast<PiranaPlant*>(eventEntity);
				if (piranaPlant->GetHealth() >= 0) {
					TakeDamage();
					_velocity.y = -_bounceSpeed;
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_BOOMERBRO:
			{
				BoomerBro* boomerBro = dynamic_cast<BoomerBro*>(eventEntity);
				if (eventNormal.y == -1.0f) {
					if (boomerBro->GetHealth() > 0) {
						boomerBro->TakeDamage();
						_velocity.y = -_bounceSpeed;
					}
				}
				else if (eventNormal.y == 1.0f || eventNormal.x != 0.0f) {
					if (boomerBro->GetHealth() > 0) {
						TakeDamage();
						_velocity.y = -_bounceSpeed;
					}
				}
			}
			break;
	//----------------------------------------------------------------------------
	//NPCs
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//PROJECTILES
	//----------------------------------------------------------------------------
		case GameObjectType::GAMEOBJECT_TYPE_VFIREBALL:
		case GameObjectType::GAMEOBJECT_TYPE_BOOMERANG:
			TakeDamage();
			minTime = { 1.0f, 1.0f };
			offset = normal = { 0, 0 };
			break;
	//----------------------------------------------------------------------------
	//PROJECTILES
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//SPECIAL ENTITIES
	//----------------------------------------------------------------------------
		case GameObjectType::GAMEOBJECT_TYPE_PORTAL:
			{
				Portal* portal = dynamic_cast<Portal*>(eventEntity);
				/*
				if (portal->GetExtraDataSize() == 1) {
					_nextSceneID = portal->GetSceneID();
					_mapNodePos = portal->GetPosition();

					minTime = { 1.0f, 1.0f };
					offset = normal = { 0, 0 };
				}
				else {
					if (Device::IsKeyDown(DIK_S) || Device::IsKeyDown(DIK_W) {
						if (eventNormal.y == 1.0f) {
							_normal.y = -1.0f;
						}
						else if (eventNormal.y == -1.0f) {
							_normal.y = 1.0f;
						}

						if (IsInPipe()) {
							StartInPipeTimer();
							_destination = portal->GetDestination();
						}
					}
				}
				*/
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_MOVINGPLATFORM:
			{
				
			}
			break;
	//----------------------------------------------------------------------------
	//SPECIAL ENTITIES
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//Items
	//----------------------------------------------------------------------------
		case GameObjectType::GAMEOBJECT_TYPE_RMUSHROOM:
		case GameObjectType::GAMEOBJECT_TYPE_GMUSHROOM:
			{
				Mushroom* mushroom = dynamic_cast<Mushroom*>(eventEntity);
				mushroom->TakeDamage();
				switch (mushroom->GetObjectType()) {
					case GameObjectType::GAMEOBJECT_TYPE_RMUSHROOM:
						if (_health <= 1) {
							_health = 2;
							_position.y -= GetBoxHeight();
						}
						break;
					case GameObjectType::GAMEOBJECT_TYPE_GMUSHROOM:
						//Stub
						break;
				}

				minTime = { 1.0f, 1.0f };
				offset = normal = { 0, 0 };
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_LEAF:
			_health = 4;
			eventEntity->TakeDamage();

			minTime = { 1.0f, 1.0f };
			offset = normal = { 0, 0 };
			break;
		case GameObjectType::GAMEOBJECT_TYPE_COIN:
			{
				Coin* coin = dynamic_cast<Coin*>(eventEntity);
				if (coin->GetHealth() == 1) {
					coin->TakeDamage();
				}
				else if (coin->GetHealth() == 3) {
					if (eventNormal.y == 1.0f) {
						//coin->SetHealth(-1);
					}
				}

				if (coin->GetHealth() != 3) {
					minTime = { 1.0f, 1.0f };
					offset = normal = { 0, 0 };
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_BONUSITEM:
			{
				BonusItem* bonusItem = dynamic_cast<BonusItem*>(eventEntity);
				if (bonusItem->GetHealth() == 2) {
					_bonusItems.emplace_back(bonusItem->GetCurrentItem());
					bonusItem->TakeDamage();
				}

				minTime = { 1.0f, 1.0f };
				offset = normal = { 0, 0 };

				_triggeredStageEnd = true;
			}
			break;
	//----------------------------------------------------------------------------
	//Items
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//Animated blocks
	//----------------------------------------------------------------------------
			case GameObjectType::GAMEOBJECT_TYPE_QUESTIONBLOCK:
				{
					QuestionBlock* questionBlock = dynamic_cast<QuestionBlock*>(eventEntity);
					if (eventNormal.y == 1.0f) {
						questionBlock->TakeDamage();
					}
				}
				break;
			case GameObjectType::GAMEOBJECT_TYPE_SHINYBRICK:
				{
					ShinyBrick* shinyBrick = dynamic_cast<ShinyBrick*>(eventEntity);
					if (shinyBrick->GetHealth() == 2) {
						if (eventNormal.y == 1.0f) {
							if (shinyBrick->GetExtraData().size() == 3) {
								if (_health > 1) {
									shinyBrick->SetHealth(-1);
								}
							}
							else {
								shinyBrick->TakeDamage();
							}
						}
					}
					//Is coin
					else if (shinyBrick->GetHealth() == 3) {
						shinyBrick->SetHealth(-1);
						minTime = { 1.0f, 1.0f };
						offset = normal = { 0, 0 };
					}
				}
				break;
			case GameObjectType::GAMEOBJECT_TYPE_PBLOCK:
				{
					PBlock* pBlock = dynamic_cast<PBlock*>(eventEntity);
					if (eventNormal.y == -1.0f) {
						pBlock->TakeDamage();
						_velocity.y = -_bounceSpeed;
					}
				}
				break;
	//----------------------------------------------------------------------------
	//Animated blocks
	//----------------------------------------------------------------------------
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

	if (_isCrouching || IsAttacking()) {
		_velocity.x = 0.0f;
	}

	//----------------------------------------------------------------------------
	//TIMERS
	//----------------------------------------------------------------------------
	if (_health != 4 || (IsFlying() && GetTickCount64() - _flyStart > _flyTime)) {
		_flyStart = 0;
	}

	if (IsInPipe() && GetTickCount64() - _inPipeStart > _inPipeTime) {
		_inPipeStart = 0;
	}

	if (IsAttacking() && GetTickCount64() - _attackStart > _attackTime) {
		SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(_tail);
		_attackStart = 0;
	}

	if (IsOnFireballCoolDown() && GetTickCount64() - _fireballCoolDownStart > _fireballCoolDownTime) {
		_fireballsCount = 0;
		_fireballCoolDownStart = 0;
	}

	if (IsInvulnerable() && GetTickCount64() - _invulnerableStart > _invulnerableTime) {
		_invulnerableStart = 0;
	}
	//----------------------------------------------------------------------------
	//TIMERS
	//----------------------------------------------------------------------------

	if (_triggeredStageEnd) {
		MoveRight();
	}

	_playerState->Update(deltaTime);
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
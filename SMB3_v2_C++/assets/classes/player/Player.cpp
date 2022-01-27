#include "../Device.h"
#include "../SceneManager.h"
#include "Player.h"
#include "state/IdleState.h"
#include "../EntityList.h"
#include "../audio/AudioService.h"

Texture* Player::_playerTexture = nullptr;
std::vector<GameObject::GameObjectType> Player::_bonusItems;

void Player::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _playerTexture);
}

void Player::_HandleCurrencies() {
	if (_lives > _MAX_LIVES) {
		_lives = _MAX_LIVES;
	}

	if (_coins > _MAX_COINS) {
		_coins = _MAX_COINS;
	}

	if (_score > _MAX_SCORE) {
		_score = _MAX_SCORE;
	}
}

void Player::_HanldeStageEnd() {
	if (_triggeredStageEnd) {
		if (!_hasBossItem) {
			_acceleration = _MIN_ACCEL;
		}

		_isHolding = false;
		
		if (_sceneRemainingTime > 0) {
			for (unsigned int i = 0; i < _sceneRemainingTime; ++i) {
				_score += 50;
			}
			_sceneRemainingTime = 0;
		}
		
	}
}

void Player::_HandleBonusItems() {
	if (_bonusItems.empty()) {
		return;
	}

	unsigned int shroomCards = 0;
	unsigned int flowerCards = 0;
	unsigned int starCards = 0;

	for (auto& item : _bonusItems) {
		switch (item) {
			case GameObjectType::GAMEOBJECT_TYPE_REDMUSHROOM:
				++shroomCards;
				break;
			case GameObjectType::GAMEOBJECT_TYPE_FLOWER:
				++flowerCards;
				break;
			case GameObjectType::GAMEOBJECT_TYPE_STAR:
				++starCards;
				break;
		}
	}

	const unsigned int MAX_ITEMS = 3;
	if (_bonusItems.size() == MAX_ITEMS) {
		if (shroomCards == MAX_ITEMS) {
			_lives += 2;
		}
		else if (flowerCards == MAX_ITEMS) {
			_lives += 3;
		}
		else if (starCards == MAX_ITEMS) {
			_lives += 5;
		}
		else {
			_lives += 1;
		}
		
		_bonusItems.clear();

		AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_1UP);
	}
}

void Player::_HandleMovementMap() {
	if (abs(_position.x - _lastPos.x) >= _MAX_TRAVEL_DISTANCE || abs(_position.y - _lastPos.y) >= _MAX_TRAVEL_DISTANCE) {
		_velocity = { 0.0f, 0.0f };
		_lastPos = _position;
		if (_mapNodePos.x != 0.0f && _mapNodePos.y != 0.0f) {
			_position = _mapNodePos;
		}
	}
}

void Player::_HandleMovementGame() {
	const int LEFT_KEY = Device::GetInstance()->GetControllerKey("LEFT");
	const int DOWN_KEY = Device::GetInstance()->GetControllerKey("DOWN");
	const int RIGHT_KEY = Device::GetInstance()->GetControllerKey("RIGHT");
	const int B_KEY = Device::GetInstance()->GetControllerKey("B");
	const int A_KEY = Device::GetInstance()->GetControllerKey("A");

	if (_isOnGround) {
		_gravity = 0.0025f;
	}

	//Variable jump height	
	if (Device::IsKeyDown(A_KEY)) {
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
			if (Device::IsKeyDown(RIGHT_KEY)) {
				_acceleration = 0.0499f;

				AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_SKID);
			}
		}
		else if (_normal.x == 1) {
			if (Device::IsKeyDown(LEFT_KEY)) {
				_acceleration = 0.0499f;

				AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_SKID);
			}
		}
	}

	if (Device::IsKeyDown(LEFT_KEY)) {
		MoveLeft();
	}
	else if (Device::IsKeyDown(RIGHT_KEY)) {
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

	if (Device::IsKeyDown(LEFT_KEY) || Device::IsKeyDown(RIGHT_KEY)) {
		//GOTTA GO FAAAST
		if (Device::IsKeyDown(B_KEY) && (_isOnGround || !IsFlying())) {
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
}

Player::Player() {
	_scale = D3DXVECTOR2(-1.0f, 1.0f);
	_renderPriority = 0;

	_runSpeed = 0.09f;
	_jumpSpeed = 0.327f;
	_bounceSpeed = 0.4f;
	_gravity = 0.0025f;
	_acceleration = 0.5f;

	_health = 1;

	_lives = 3;
	_coins = 0;
	_score = 0;

	_fireballsCount = 0;

	_upVector = 1.0f;

	_heldEntity = nullptr;

	_flyTime = 6000;
	_inPipeTime = 2000;
	_attackTime = 150;
	_fireballCoolDownTime = 2500;
	_invulnerableTime = 1000;

	_playerState = new IdleState(this);

	_bonusItems.reserve(3);
}

Player::~Player() {}

unsigned int Player::GetNextSceneID() const {
	return _nextSceneID;
}

RECTF Player::GetBoundingBox(int index) const {
	return GameObject::GetBoundingBox(_health >= 2 && !isInMap && !_isCrouching);
}

Entity* Player::GetHeldEntity() const {
	return _heldEntity;
}

void Player::GetSceneRemainingTime(unsigned int sceneTime) {
	_sceneRemainingTime = sceneTime;
}

void Player::SetUpVector(float upVector) {
	if (upVector == 0.0f) {
		return;
	}
	
	_upVector = upVector;
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
	if (isInMap) {	
		_HandleMovementMap();
	}
	else {
		_HandleMovementGame();
	}

	PlayerState* currentState = _playerState->HandleStates();
	if (currentState != nullptr) {
		delete _playerState;
		_playerState = currentState;
	}
}

void Player::OnKeyUpMap(int keyCode) {
	//Stub
}

void Player::OnKeyUpGame(int keyCode) {
	const int DOWN_KEY = Device::GetInstance()->GetControllerKey("DOWN");
	const int B_KEY = Device::GetInstance()->GetControllerKey("B");

	if (keyCode == DOWN_KEY) {
		_isCrouching = false;

		if (_health > 1 && _isOnGround && !IsInPipe()) {
			_isOnGround = false;
			_position.y = ceil(_position.y - _CROUCH_HEIGHT_ADJUST);
		}
	}
	else if (keyCode == B_KEY) {
		_isHolding = false;
	}
}

void Player::OnKeyDownMap(int keyCode) {
	const float MAP_RUN_SPEED = 0.08f;

	const int UP_KEY = Device::GetInstance()->GetControllerKey("UP");
	const int LEFT_KEY = Device::GetInstance()->GetControllerKey("LEFT");
	const int DOWN_KEY = Device::GetInstance()->GetControllerKey("DOWN");
	const int RIGHT_KEY = Device::GetInstance()->GetControllerKey("RIGHT");

	if (keyCode == UP_KEY) {
		_velocity.y = -MAP_RUN_SPEED;
	}
	else if (keyCode == LEFT_KEY) {
		_velocity.x = -MAP_RUN_SPEED;
	}
	else if (keyCode == DOWN_KEY) {
		_velocity.y = MAP_RUN_SPEED;
	}
	else if (keyCode == RIGHT_KEY) {
		_velocity.x = MAP_RUN_SPEED;
	}
}

void Player::OnKeyDownGame(int keyCode) {
	const int LEFT_KEY = Device::GetInstance()->GetControllerKey("LEFT");
	const int DOWN_KEY = Device::GetInstance()->GetControllerKey("DOWN");
	const int RIGHT_KEY = Device::GetInstance()->GetControllerKey("RIGHT");
	const int B_KEY = Device::GetInstance()->GetControllerKey("B");
	const int A_KEY = Device::GetInstance()->GetControllerKey("A");

	if (keyCode == LEFT_KEY) {
		_normal.x = -1.0f;
	}
	else if (keyCode == RIGHT_KEY) {
		_normal.x = 1.0f;
	}
	else if (keyCode == DOWN_KEY) {
		_isCrouching = true;

		if (_health > 1 && _isOnGround && !IsInPipe()) {
			_position.y = ceil(_position.y + _CROUCH_HEIGHT_ADJUST);
		}
	}
	else if (keyCode == B_KEY) {
		_isHolding = true;
		//Fireball attack
		if (_health == 3 && !_isCrouching) {
			if (_fireballsCount < _FIREBALLS_LIMIT) {
				SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(SpawnFireball());
				++_fireballsCount;

				if (_fireballsCount == _FIREBALLS_LIMIT) {
					StartFireballCoolDownTimer();
				}

				AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_FIREBALL);
			}
		}

		//Tail attack
		if (_health == 4 && !IsAttacking()) {
			StartAttackTimer();

			AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_TAILATTACK);
		}
	}
	else if (keyCode == A_KEY) {
		SlowFall();
		RunFly();
		Jump();
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
		_originalVel = { _velocity.x, _isOnGround ? _velocity.y : -_bounceSpeed };

		StartInvulnerableTimer();

		if (_health > 2) {
			_health = 2;

			AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_TRANSFORM);
		}
		else {
			--_health;

			AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_PIPE);

			if (_health < 1) {
				--_lives;

				AudioService::GetAudio().StopAll();
				AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_DEATH);
			}
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

		AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_JUMP);
	}
}

void Player::RunFly() {
	if (_health == 4) {
		if (_acceleration >= _ACCEL_THRESHOLD || IsFlying()) {
			if (_isOnGround && !IsFlying()) {
				_isOnGround = false;
				StartFlyTimer();

				AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_PMETER, true);
			}
			
			const float FLOAT_MODIFIER = 0.66f;
			_velocity.y = -_jumpSpeed * FLOAT_MODIFIER;
		}
	}
}

void Player::SlowFall() {
	if (_health == 4 && !_isOnGround) {
		const float SLOW_MODIFIER = 0.2f;
		_velocity.y *= SLOW_MODIFIER;

		AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_TAILATTACK, false);
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
	const float OFFSET = 10.0f;
	fireball->SetNormal({ _normal.x, fireball->GetNormal().y });
	fireball->SetPosition({ _position.x, _position.y + OFFSET });
	return fireball;
}

void Player::HandleCollisionResult(
	LPCOLLISIONEVENT result, 
	D3DXVECTOR2& minTime, 
	D3DXVECTOR2& offset, 
	D3DXVECTOR2& normal, 
	D3DXVECTOR2& relativeDistance) 
{
	const int UP_KEY = Device::GetInstance()->GetControllerKey("UP");
	const int LEFT_KEY = Device::GetInstance()->GetControllerKey("LEFT");
	const int DOWN_KEY = Device::GetInstance()->GetControllerKey("DOWN");
	const int RIGHT_KEY = Device::GetInstance()->GetControllerKey("RIGHT");

	Entity* eventEntity = result->entity;
	D3DXVECTOR2 eventNormal = result->normal;

	if (IsInPipe() || IsInvulnerable()) {
		if (IsInPipe()) {
			minTime = { 1.0f, 1.0f };
			offset = normal = { 0.0f, 0.0f };
		}
		return;
	}

	switch (eventEntity->GetObjectType()) {
		case GameObjectType::GAMEOBJECT_TYPE_PORTAL:
		case GameObjectType::GAMEOBJECT_TYPE_MOVINGPLATFORM:
		case GameObjectType::GAMEOBJECT_TYPE_QUESTIONBLOCK:
		case GameObjectType::GAMEOBJECT_TYPE_TILE:
		case GameObjectType::GAMEOBJECT_TYPE_ONEWAYPLATFORM:
			if (eventNormal.y == -1.0f) {
				_isOnGround = true;
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_COIN:
			//Is brick
			if (eventEntity->GetHealth() == 3) {
				_isOnGround = true;
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_SHINYBRICK:
			//Is coin
			if (eventEntity->GetHealth() != 3) {
				_isOnGround = true;
			}
			break;
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

						AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_SQUISH);
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

						AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_SQUISH);
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

						AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_KICK);
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

							AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_KICK);
						}
					}
					else {
						TakeDamage();
						_velocity.y = -_bounceSpeed;
					}
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_PIRANHAPLANT:
		case GameObjectType::GAMEOBJECT_TYPE_VENUSPLANT:
			{
				PiranaPlant* piranaPlant = dynamic_cast<PiranaPlant*>(eventEntity);
				if (piranaPlant->GetHealth() >= 0) {
					TakeDamage();
					_velocity.y = -_bounceSpeed;
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_BOOMERANGBRO:
			{
				BoomerBro* boomerBro = dynamic_cast<BoomerBro*>(eventEntity);
				if (eventNormal.y == -1.0f) {
					if (boomerBro->GetHealth() > 0) {
						boomerBro->TakeDamage();
						_velocity.y = -_bounceSpeed;

						AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_SQUISH);
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
		case GameObjectType::GAMEOBJECT_TYPE_PODOBOO:
			{
				Podoboo* podoboo = dynamic_cast<Podoboo*>(eventEntity);
				TakeDamage();
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_DRYBONES:
			{
				DryBones* dryBones = dynamic_cast<DryBones*>(eventEntity);
				if (eventNormal.y == -1.0f) {
					if (dryBones->GetHealth() == 3) {
						_velocity.y = -_bounceSpeed;

						dryBones->TakeDamage();

						AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_BLOCKBREAK);
					}
				}
				else if (eventNormal.x != 0.0f || eventNormal.y == 1.0f) {
					if (dryBones->GetHealth() == 3) {
						TakeDamage();
						_velocity.y = -_bounceSpeed;
					}
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_FORTRESSBOSS:
			{
				FortressBoss* fortressBoss = dynamic_cast<FortressBoss*>(eventEntity);
				if (!(fortressBoss->IsInIntro() || fortressBoss->IsInvulnerable())) {
					if (eventNormal.y == -1.0f) {
						if (fortressBoss->GetHealth() == 3 && fortressBoss->IsAttacking()) {
							TakeDamage();
						}
						else {
							fortressBoss->TakeDamage();
							_velocity.y = -_bounceSpeed;

							AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_SQUISH);
						}
					}
					else if (eventNormal.x != 0.0f || eventNormal.y == 1.0f) {
						if (fortressBoss->GetHealth() > 0) {
							TakeDamage();
						}
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
		case GameObjectType::GAMEOBJECT_TYPE_VENUSFIREBALL:
		case GameObjectType::GAMEOBJECT_TYPE_BOOMERANG:
			TakeDamage();
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
				if (portal->GetExtraData().size() == 1) {
					_nextSceneID = portal->GetSceneID();
					_mapNodePos = portal->GetPosition();
				}
				else {
					if (Device::IsKeyDown(DOWN_KEY) || Device::IsKeyDown(UP_KEY)) {
						if (eventNormal.y == 1.0f && Device::IsKeyDown(UP_KEY)) {
							_normal.y = -1.0f;
						}
						else if (eventNormal.y == -1.0f && Device::IsKeyDown(DOWN_KEY)) {
							_normal.y = 1.0f;
						}

						if (!IsInPipe()) {
							StartInPipeTimer();
							_destination = portal->GetDestination();

							AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_PIPE);
						}
					}
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_MOVINGPLATFORM:
			{
				//Stub
			}
			break;
	//----------------------------------------------------------------------------
	//SPECIAL ENTITIES
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//ITEMS
	//----------------------------------------------------------------------------
		case GameObjectType::GAMEOBJECT_TYPE_REDMUSHROOM:
		case GameObjectType::GAMEOBJECT_TYPE_GREENMUSHROOM:
			{
				Mushroom* mushroom = dynamic_cast<Mushroom*>(eventEntity);
				mushroom->TakeDamage();
				switch (mushroom->GetObjectType()) {
					case GameObjectType::GAMEOBJECT_TYPE_REDMUSHROOM:
						if (_health == 1) {
							_health = 2;
							_position.y -= GetBoxHeight();

							if (!IsInvulnerable()) {
								_originalVel = _velocity;

								StartInvulnerableTimer();
							}
						}

						AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_POWERUP);
						break;
					case GameObjectType::GAMEOBJECT_TYPE_GREENMUSHROOM:
						AudioService::GetAudio().PlayAudio(IsAttacking() ? AudioType::AUDIO_TYPE_1UPGLITCH : AudioType::AUDIO_TYPE_1UP);
						break;
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_LEAF:
			{
				Leaf* leaf = dynamic_cast<Leaf*>(eventEntity);
				leaf->TakeDamage();

				if (_health == 4) {
					AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_POWERUP);
				}
				else {
					if (_health == 1) {
						_health = 2;
					
						AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_POWERUP);
					}
					else {
						_health = 4;
					
						AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_TRANSFORM);
					}

					if (!IsInvulnerable()) {
						_originalVel = _velocity;

						StartInvulnerableTimer();
					}
				}				
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_FLOWER:
			{
				Flower* flower = dynamic_cast<Flower*>(eventEntity);
				flower->TakeDamage();

				if (_health == 3) {
					AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_POWERUP);
				}
				else {
					if (_health == 1) {
						_health = 2;

						AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_POWERUP);
					}
					else {
						_health = 3;

						AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_TRANSFORM);
					}

					if (!IsInvulnerable()) {
						_originalVel = _velocity;

						StartInvulnerableTimer();
					}
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_COIN:
			{
				Coin* coin = dynamic_cast<Coin*>(eventEntity);
				if (coin->GetHealth() == 1) {
					coin->TakeDamage();

					AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_COIN);
				}
				//Is brick
				else if (coin->GetHealth() == 3) {
					if (eventNormal.y == 1.0f) {
						coin->SetHealth(-1);
					}
				}
			}
			break;
	//----------------------------------------------------------------------------
	//ITEMS
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//ANIMATED BLOCKS
	//----------------------------------------------------------------------------
			case GameObjectType::GAMEOBJECT_TYPE_QUESTIONBLOCK:
				{
					QuestionBlock* questionBlock = dynamic_cast<QuestionBlock*>(eventEntity);
					if (eventNormal.y == 1.0f) {
						questionBlock->TakeDamage();

						AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_BUMP);
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

									AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_BLOCKBREAK);
								}
							}
							else {
								shinyBrick->TakeDamage();
							}

							AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_BUMP);
						}
					}
					//Is coin
					else if (shinyBrick->GetHealth() == 3) {
						shinyBrick->SetHealth(-2);
						
						_coins += 1;

						AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_COIN);
					}
				}
				break;
			case GameObjectType::GAMEOBJECT_TYPE_PBLOCK:
				{
					PBlock* pBlock = dynamic_cast<PBlock*>(eventEntity);
					if (eventNormal.y == -1.0f) {
						pBlock->TakeDamage();
						_velocity.y = -_bounceSpeed;

						AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_THWOMP);
					}
				}
				break;
			case GameObjectType::GAMEOBJECT_TYPE_LAVAPOOL:
				{
					LavaPool* lavaPool = dynamic_cast<LavaPool*>(eventEntity);
					if (eventNormal.x != 0.0f || eventNormal.y != 0.0f) {
						_health = 1;
						TakeDamage();
					}
				}
				break;
			case GameObjectType::GAMEOBJECT_TYPE_DOOR:
				{
					Door* door = dynamic_cast<Door*>(eventEntity);
					if (Device::IsKeyDown(UP_KEY)) {
						_position = door->GetDestination();

						_wentIntoPipe = !_wentIntoPipe;
						door->canTriggerMovingCeiling = !door->canTriggerMovingCeiling;
					}
				}
				break;
	//----------------------------------------------------------------------------
	//ANIMATED BLOCKS
	//----------------------------------------------------------------------------
			case GameObjectType::GAMEOBJECT_TYPE_ONEHITPLATFORM:
				_position.y = static_cast<float>(SceneManager::GetInstance()->GetCurrentScene()->GetSceneHeight());
				_health = 1;
				TakeDamage();
				break;
			case GameObjectType::GAMEOBJECT_TYPE_TILE:
				if (eventNormal.x != 0.0f) {
					if (Device::IsKeyDown(LEFT_KEY) || Device::IsKeyDown(RIGHT_KEY)) {
						if (GetTickCount64() % 100 == 0) {
							AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_BUMP);
						}
					}
				}
				break;
			case GameObjectType::GAMEOBJECT_TYPE_MOVINGCEILING:
				if (eventNormal.y == 1.0f) {
					_health = 1;
					TakeDamage();
				}
				break;
	}
}

void Player::HandleOverlap(Entity* entity) {
	switch (entity->GetObjectType()) {
		case GameObjectType::GAMEOBJECT_TYPE_ROTODISC:
			{
				Rotodisc* rotodisc = dynamic_cast<Rotodisc*>(entity);
				TakeDamage();
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_BONUSITEM:
			{
				BonusItem* bonusItem = dynamic_cast<BonusItem*>(entity);
				if (bonusItem->GetHealth() == 2) {
					_bonusItems.emplace_back(bonusItem->GetCurrentItem());
					bonusItem->TakeDamage();
				}

				_triggeredStageEnd = true;

				AudioService::GetAudio().StopAll();
				AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_STAGE_END);
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_ORB:
			{
				Orb* orb = dynamic_cast<Orb*>(entity);
				orb->TakeDamage();

				_triggeredStageEnd = true;
				_hasBossItem = true;

				AudioService::GetAudio().StopAll();
				AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_BATTLE_CLEAR);
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_TRIGGER:
			{
				Trigger* trigger = dynamic_cast<Trigger*>(entity);
				trigger->triggered = true;
				trigger->SetPosition({ 0.0f, 0.0f });

				lockCameraXAxis = true;

				AudioService::GetAudio().StopAll();
				AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_BATTLE_MINIBOSS);
			}
			break;
	}
}

void Player::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	GlobalUtil::debugStruct.playerPosition = _position;

	_HandleCurrencies();
	_HanldeStageEnd();
	_HandleBonusItems();

	//----------------------------------------------------------------------------
	//TIMERS
	//----------------------------------------------------------------------------
	if (_health != 4 || (IsFlying() && GetTickCount64() - _flyStart > _flyTime)) {
		_flyStart = 0;

		AudioService::GetAudio().StopAudio(AudioType::AUDIO_TYPE_PMETER);
	}

	if (IsInPipe() && GetTickCount64() - _inPipeStart > _inPipeTime) {
		_inPipeStart = 0;
	}

	if (IsAttacking() && GetTickCount64() - _attackStart > _attackTime) {
		_attackStart = 0;
	}

	if (IsOnFireballCoolDown() && GetTickCount64() - _fireballCoolDownStart > _fireballCoolDownTime) {
		_fireballsCount = 0;
		_fireballCoolDownStart = 0;
	}

	if (IsInvulnerable() && GetTickCount64() - _invulnerableStart > _invulnerableTime) {
		_invulnerableStart = 0;

		_velocity = _originalVel;
	}
	//----------------------------------------------------------------------------
	//TIMERS
	//----------------------------------------------------------------------------

	//To show the whole kicking animation
	if (_isNextToShell && GetTickCount64() % 500 == 0) {
		_isNextToShell = false;
	}

	if (_health <= 0 || _isCrouching) {
		_velocity.x = 0.0f;
	}

	if (_triggeredStageEnd && !_hasBossItem) {
		MoveRight();
	}

	if (IsInPipe()) {
		_velocity.y = _health > 1 ? 0.035f : 0.03f;
		_velocity.y *= _normal.y;

		_isHolding = false;
		_velocity.x = 0.0f;

		if (GetTickCount64() - _inPipeStart == _inPipeTime * 0.5f) {
			_wentIntoPipe = !_wentIntoPipe;
			_position = _destination;
			_isOnGround = false;
			_normal.y *= _upVector;

			AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_PIPE);
		}
	}

	if (IsInvulnerable()) {
		if (GetTickCount64() - _invulnerableStart >= _invulnerableTime * 0.82f) {
			//Death bounce
			if (_health <= 0) {
				_isOnGround = false;
				_isHolding = false;

				_velocity.y = -_bounceSpeed;
				_gravity = 0.0010f;
			}
		}
		else {
			//Freeze the player
			_velocity = { 0.0f, 0.0f };
		}
	}

	_playerState->Update(deltaTime);
	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);

	if (_heldEntity != nullptr) {
		if (_heldEntity->GetHealth() == 0 || _heldEntity->GetHealth() == 3) {
			_isHolding = false;

			if (_heldEntity->GetHealth() == 3) {
				const float OFFSET_X = 17.0f;
				const float OFFSET_Y = 14.0f;
				_heldEntity->SetPosition(
					{ 
					_position.x + OFFSET_X * _normal.x, 
					_position.y - OFFSET_Y 
					}
				);
			}

			_heldEntity->isBeingHeld = false;
			_heldEntity = nullptr;
			return;
		}
	
		if (_isHolding) {
			D3DXVECTOR2 offset;
			offset.x = IsInPipe() ? 0.0f : 12.0f;
			offset.y = _health == 1 ? 2.0f : -3.0f;

			_heldEntity->SetPosition({ _position.x + offset.x * _normal.x, _position.y - offset.y });
		}
		else {
			_isNextToShell = true;

			_heldEntity->TakeDamage();
			_heldEntity->SetNormal({ -_normal.x, 0.0f });
			_heldEntity->isBeingHeld = false;
			_heldEntity = nullptr;

			AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_KICK);
		}
	}
}

void Player::Render() {
	_playerState->Render();
}

void Player::Release() {
	if (_playerState != nullptr) {
		_playerState->Release();
		delete _playerState;
	}

	_animatedSprite.Release();
	_playerTexture = nullptr;
}
#include "../SceneManager.h"
#include "../Entity.h"
#include "Koopa.h"
#include "../EntityList.h"
#include "../audio/AudioService.h"

Texture* Koopa::_koopaTexture = nullptr;

void Koopa::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _koopaTexture);
}

Koopa::Koopa() {
	_renderPriority = 1;

	_health = 3;

	_runSpeed = 0.03f;
	_jumpSpeed = 0.22f;
	_gravity = 0.001f;

	_variant = "red";
	_state = _State::WALK;
	_retractTime = 8000;
}

Koopa::~Koopa() {}

bool Koopa::IsRetracting() const {
	return _retractStart != 0;
}

void Koopa::StartRetractTimer() {
	_retractStart = static_cast<DWORD>(GetTickCount64());
}

RECTF Koopa::GetBoundingBox(int index) const {
	return _health <= 0 ? RECTF() : GameObject::GetBoundingBox(_health < 3);
}

void Koopa::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_koopaTexture == nullptr) {
		_koopaTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);

	if (_extraData.size() == 1) {
		_variant = _extraData.front();
	}
}

void Koopa::TakeDamage() {
	if (_health >= 2) {
		--_health;
		tookDamage = true;
		StartRetractTimer();
	}
}

void Koopa::HandleStates() {
	_state = static_cast<_State>(_health);

	switch (_state) {
		case _State::FLY:
		case _State::WALK:
			_velocity.x = -_runSpeed * _normal.x;
			break;
		case _State::RETRACT:
			_velocity.x = 0.0f;
			break;
		case _State::SPIN:
			{
				const float SPEED_MODIFIER = 6.0f;
				_velocity.x = -_runSpeed * _normal.x * SPEED_MODIFIER;
			}
			break;
		case _State::DIE:
			_isOnGround = false;

			if (_health == 0 && !IsRemoved()) {
				StartRemoveTimer();
			}
			break;
	}
}

void Koopa::HandleCollisionResult(
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

	switch (eventEntity->GetObjectType()) {
		case GameObjectType::GAMEOBJECT_TYPE_GOOMBA:
		case GameObjectType::GAMEOBJECT_TYPE_PARAGOOMBA:
			{
				Goomba* goomba = dynamic_cast<Goomba*>(eventEntity);
				if (_state == _State::SPIN) {
					goomba->animationName = "Walk";
					goomba->SetHealth(0);
					goomba->SetScale({ 1.0f, -1.0f });
					goomba->SetVelocity({ 0.0f, -_jumpSpeed });

					AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_KICK);
				}
				else {
					if (eventNormal.x != 0.0f) {
						_normal.x = -_normal.x;
					}
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_KOOPA:
		case GameObjectType::GAMEOBJECT_TYPE_PARAKOOPA:
			{
				Koopa* koopa = dynamic_cast<Koopa*>(eventEntity);
				if (_state == _State::SPIN || isBeingHeld) {
					koopa->SetHealth(0);
					koopa->SetScale({ 1.0f, -1.0f });
					koopa->SetVelocity({ _runSpeed * koopa->GetNormal().x, -_jumpSpeed });

					_health = 0;
					tookDamage = true;
					_scale.y = -1.0f;
					_velocity.x = -_runSpeed * koopa->GetNormal().x;
					_velocity.y = -_jumpSpeed;

					AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_KICK);
				}
				else {
					if (eventNormal.x != 0.0f) {
						_normal.x = -_normal.x;
					}
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_PIRANHAPLANT:
		case GameObjectType::GAMEOBJECT_TYPE_VENUSPLANT:
			{
				PiranaPlant* piranaPlant = dynamic_cast<PiranaPlant*>(eventEntity);
				if (_state == _State::SPIN) {
					piranaPlant->TakeDamage();

					AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_KICK);
				}
				else {
					if (eventNormal.x != 0.0f) {
						_normal.x = -_normal.x;
					}
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_BOOMERANGBRO:
			{
				BoomerBro* boomerBro = dynamic_cast<BoomerBro*>(eventEntity);
				if (_state == _State::SPIN) {
					boomerBro->TakeDamage();
					boomerBro->SetVelocity({ 0.0f, -_jumpSpeed });

					AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_KICK);
				}
				else {
					if (eventNormal.x != 0.0f) {
						_normal.x = -_normal.x;
					}
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_COIN:
			{
				Coin* coin = dynamic_cast<Coin*>(eventEntity);
				//Is brick
				if (coin->GetHealth() == 3) {
					if (eventNormal.x != 0.0f) {
						_normal.x = -_normal.x;
					}
					
					if (_state == _State::SPIN && eventNormal.x != 0.0f) {
						coin->SetHealth(-1);
					}
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_QUESTIONBLOCK:
			{
				QuestionBlock* questionBlock = dynamic_cast<QuestionBlock*>(eventEntity);				
				if (_state == _State::SPIN && eventNormal.x != 0.0f) {
					questionBlock->TakeDamage();

					AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_BUMP);
				}
			}

			if (eventNormal.x != 0.0f) {
				_normal.x = -_normal.x;
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_SHINYBRICK:
			{
				ShinyBrick* shinyBrick = dynamic_cast<ShinyBrick*>(eventEntity);
				if (shinyBrick->GetHealth() != 3 && eventNormal.x != 0.0f) {
					_normal.x = -_normal.x;
				}
				
				if (_state == _State::SPIN && eventNormal.x != 0.0f) {
					//Has items
					if (shinyBrick->GetExtraData().size() != 3) {
						shinyBrick->TakeDamage();

						AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_BUMP);
					}
					//Is empty
					else if (shinyBrick->GetHealth() != 3 && shinyBrick->GetExtraData().size() == 3) {
						shinyBrick->SetHealth(-1);

						AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_BLOCKBREAK);
					}
				}
				else if (_state != _State::SPIN && shinyBrick->GetHealth() != 3){
					if (_position.x <= shinyBrick->GetPosition().x - 5.0f) {
						_normal.x = -1.0f;

					}
					else if (_position.x + _hitbox.GetBoxWidth() >= shinyBrick->GetPosition().x + shinyBrick->GetBoxWidth() + 5.0f) {
						_normal.x = 1.0f;
					}

					if (eventNormal.x != 0.0f) {
						_normal.x = -_normal.x;
					}
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_PBLOCK:
			{
				PBlock* pBlock = dynamic_cast<PBlock*>(eventEntity);
				if (_state == _State::SPIN) {
					pBlock->TakeDamage();

					AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_THWOMP);
				}
			}

			if (eventNormal.x != 0.0f) {
				_normal.x = -_normal.x;
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_ONEHITPLATFORM:
			_position.y = static_cast<float>(SceneManager::GetInstance()->GetCurrentScene()->GetSceneHeight());
			break;
		case GameObjectType::GAMEOBJECT_TYPE_TILE:
		case GameObjectType::GAMEOBJECT_TYPE_ONEWAYPLATFORM:	
			if (_variant == "red") {
				if (_state != _State::SPIN) {
					if (_position.x <= eventEntity->GetPosition().x - 5.0f) {
						_normal.x = -1.0f;

					}
					else if (_position.x + _hitbox.GetBoxWidth() >= eventEntity->GetPosition().x + eventEntity->GetBoxWidth() + 5.0f) {
						_normal.x = 1.0f;
					}
				}
			}

			if (eventNormal.x != 0.0f) {
				_normal.x = -_normal.x;

				if (_state == _State::SPIN) {
					AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_BUMP);
				}
			}
			break;
	}
}

void Koopa::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (_state == _State::SPIN) {
		StartRetractTimer();
	}

	if (IsRetracting() && GetTickCount64() - _retractStart > _retractTime) {
		_health = 3;
		_scale.y = 1.0f;
		_position.y -= _hitbox.GetBoxHeight();
		_retractStart = 0;
	}

	HandleStates();
	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);
}

void Koopa::Render() {
	switch (_state) {
		case _State::FLY:
			_animatedSprite.PlaySpriteAnimation("Fly", _position, { _normal.x, _scale.y });
			break;
		case _State::WALK:
			_animatedSprite.PlaySpriteAnimation("Walk", _position, { _normal.x, _scale.y });
			break;
		case _State::RETRACT:
			_animatedSprite.PlaySpriteAnimation("ShellIdle", _position, _scale);

			if (GetTickCount64() - _retractStart > _retractTime * 0.75f) {
				_animatedSprite.PlaySpriteAnimation("ShellWake", _position, _scale);
			}
			break;
		case _State::SPIN:
			_animatedSprite.PlaySpriteAnimation("ShellSpin", _position, _scale);
			break;
		case _State::DIE:
			_animatedSprite.PlaySpriteAnimation("ShellIdle", _position, _scale);
			break;
	}
}

void Koopa::Release() {
	_animatedSprite.Release();
	_koopaTexture = nullptr;
}
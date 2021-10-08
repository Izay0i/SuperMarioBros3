#include "../Entity.h"
#include "Koopa.h"

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

	_variant = "green";
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
			_velocity.x = -_runSpeed * _normal.x * 6.0f;
			break;
		case _State::DIE:
			_velocity.x = 0.0f;
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

	if (eventEntity == nullptr) {
		return;
	}

	if (eventNormal.y == -1.0f) {
		_isOnGround = true;
	}

	switch (eventEntity->GetObjectType()) {
		case GameObjectType::GAMEOBJECT_TYPE_TAIL:
			TakeDamage();
			_scale.y = -1.0f;
			_velocity.y = -_bounceSpeed;
			break;
		case GameObjectType::GAMEOBJECT_TYPE_VFIREBALL:
		case GameObjectType::GAMEOBJECT_TYPE_RMUSHROOM:
		case GameObjectType::GAMEOBJECT_TYPE_GMUSHROOM:
		case GameObjectType::GAMEOBJECT_TYPE_LEAF:
		case GameObjectType::GAMEOBJECT_TYPE_COIN:
		case GameObjectType::GAMEOBJECT_TYPE_BONUSITEM:
			minTime = { 1.0f, 1.0f };
			offset = normal = relativeDistance = { 0, 0 };
			break;
		//Kill both of them if one is spinning or is being held by the player;
		case GameObjectType::GAMEOBJECT_TYPE_KOOPA:
		case GameObjectType::GAMEOBJECT_TYPE_PARAKOOPA:
			{
				Koopa* koopa = dynamic_cast<Koopa*>(eventEntity);
				if (koopa->_state == _State::SPIN || isBeingHeld) {
					koopa->SetHealth(0);
					koopa->SetScale({ 1.0f, -1.0f });
					koopa->SetVelocity({ 0.0f, -_jumpSpeed });

					_health = 0;
					tookDamage = true;
					_scale.y = -1.0f;
					_velocity.y = -_jumpSpeed;
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_MOVINGPLATFORM:
			//Ignore
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
			break;
		default:
			if (_state == _State::WALK || _state == _State::SPIN) {
				if (eventNormal.x != 0.0f) {
					eventEntity->TakeDamage();
				}
			}
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
			_animatedSprite.PlaySpriteAnimation(_variant == "red" ? "RedFly" : "GreenFly", _position, { _normal.x, _scale.y });
			break;
		case _State::WALK:
			_animatedSprite.PlaySpriteAnimation(_variant == "red" ? "RedWalk" : "GreenWalk", _position, { _normal.x, _scale.y });
			break;
		case _State::RETRACT:
			_animatedSprite.PlaySpriteAnimation(_variant == "red" ? "RedShellIdle" : "GreenShellIdle", _position, _scale);

			if (GetTickCount64() - _retractStart > _retractTime * 0.75f) {
				_animatedSprite.PlaySpriteAnimation(_variant == "red" ? "RedShellWake" : "GreenShellWake", _position, _scale);
			}
			break;
		case _State::SPIN:
			_animatedSprite.PlaySpriteAnimation(_variant == "red" ? "RedShellSpin" : "GreenShellSpin", _position);
			break;
		case _State::DIE:
			_animatedSprite.PlaySpriteAnimation(_variant == "red" ? "RedShellIdle" : "GreenShellIdle", _position, _scale);
			break;
	}
}

void Koopa::Release() {
	_animatedSprite.Release();
	_koopaTexture = nullptr;
}
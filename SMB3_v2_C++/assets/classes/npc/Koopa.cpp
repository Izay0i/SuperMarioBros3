#include "../Entity.h"
#include "Koopa.h"

LPDIRECT3DTEXTURE9 Koopa::_koopaTexture = nullptr;

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
	const LPDIRECT3DTEXTURE9& texture, 
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
	switch (_health) {
		case 0:
			_state = _State::DIE;
			break;
		case 1:
			_state = _State::SPIN;
			break;
		case 2:
			_state = _State::RETRACT;
			break;
		case 3:
			_state = _State::WALK;
			break;
		case 4:
			_state = _State::FLY;
			break;
	}

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
	}
}

void Koopa::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (_health == 1) {
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
	if (!_isActive) {
		return;
	}

	switch (_state) {
		case _State::FLY:
			_animatedSprite.PlaySpriteAnimation(_variant == "red" ? "RedFly" : "GreenFly", _position, { _normal.x, _scale.y });
			break;
		case _State::WALK:
			_animatedSprite.PlaySpriteAnimation(_variant == "red" ? "RedWalk" : "GreenWalk", _position, { _normal.x, _scale.y });
			break;
		case _State::RETRACT:
			_animatedSprite.PlaySpriteAnimation(_variant == "red" ? "RedShellIdle" : "GreenShellIdle", _position, _scale);

			if (GetTickCount64() - _retractStart > _retractTime * 3 / 4) {
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
	_koopaTexture = nullptr;
}
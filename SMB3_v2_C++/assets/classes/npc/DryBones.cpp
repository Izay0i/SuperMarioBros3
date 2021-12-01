#include "../Entity.h"
#include "DryBones.h"

Texture* DryBones::_dryTexture = nullptr;

void DryBones::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _dryTexture);
}

DryBones::DryBones() {
	_renderPriority = 1;

	_health = 3;

	_runSpeed = 0.03f;
	_jumpSpeed = 0.22f;
	_gravity = 0.001f;

	_state = _State::WALK;
	_limboTime = 8000;
}

DryBones::~DryBones() {}

bool DryBones::IsInLimbo() const {
	return _limboStart != 0;
}

void DryBones::StartLimboTimer() {
	_limboStart = static_cast<DWORD>(GetTickCount64());
}

void DryBones::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_dryTexture == nullptr) {
		_dryTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void DryBones::TakeDamage() {
	if (_health >= 2) {
		--_health;
		tookDamage = true;
		StartLimboTimer();
	}
}

void DryBones::HandleStates() {
	_state = static_cast<_State>(_health);

	isPassThroughable = _state != _State::WALK;

	switch (_state) {
		case _State::WALK:
			_velocity.x = -_runSpeed * _normal.x;
			break;
	}
}

void DryBones::HandleCollisionResult(
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
		case GameObjectType::GAMEOBJECT_TYPE_TILE:
			if (eventNormal.x != 0.0f) {
				_normal.x = -_normal.x;
			}
			break;
	}
}

void DryBones::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid)
{
	if (IsInLimbo() && GetTickCount64() - _limboStart > _limboTime) {
		_health = 3;
		_position.y -= _hitbox.GetBoxHeight();
		_limboStart = 0;
	}

	HandleStates();
	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);
}

void DryBones::Render() {
	switch (_state) {
		case _State::WALK:
			_animatedSprite.PlaySpriteAnimation("Walk", _position, _scale);
			break;
		case _State::LIMBO:
			_animatedSprite.PlaySpriteAnimation("Die", _position, _scale);

			if (GetTickCount64() - _limboStart > _limboTime * 0.98f) {
				_animatedSprite.PlaySpriteAnimation("Revive", _position, _scale);
			}
			break;
	}
}

void DryBones::Release() {
	_animatedSprite.Release();
	_dryTexture = nullptr;
}
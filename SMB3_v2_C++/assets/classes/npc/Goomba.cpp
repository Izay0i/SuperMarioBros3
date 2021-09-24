#include "../Entity.h"
#include "Goomba.h"

Texture* Goomba::_goombaTexture = nullptr;

void Goomba::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _goombaTexture);
}

Goomba::Goomba() {
	_renderPriority = 1;

	_runSpeed = 0.03f;
	_jumpSpeed = 0.5f;
	_bounceSpeed = 0.23f;
	_gravity = 0.002f;
	
	_variant = "yellow";
	_state = _State::WALK;
}

Goomba::~Goomba() {}

RECTF Goomba::GetBoundingBox(int index) const {
	return _health <= 0 ? RECTF() : GameObject::GetBoundingBox();
}

void Goomba::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_goombaTexture == nullptr) {
		_goombaTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
	
	if (_extraData.size() == 1) {
		_variant = _extraData.front();
	}
	_animationName = _variant == "red" ? "RedDie" : "YellowDie";
}

void Goomba::HandleStates() {
	_state = static_cast<_State>(_health);

	switch (_state) {
		case _State::FLY:
		case _State::WALK:
			_velocity.x = -_runSpeed * _normal.x;
			break;
		case _State::DIE:
			_velocity.x = 0;
			_isOnGround = false;
			
			if (_animationName == "YellowDie" || _animationName == "RedDie") {
				_velocity.y = 0;
			}

			if (_health == 0 && !IsRemoved()) {
				StartRemoveTimer();
			}
			break;
	}
}

void Goomba::HandleCollisionResult(
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
			_animationName = _variant == "red" ? "RedWalk" : "YellowWalk";
			_scale.y = -1.0f;
			_velocity.y = -_bounceSpeed;
			break;
	}
}

void Goomba::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	HandleStates();
	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);
}

void Goomba::Render() {
	if (!_isActive) {
		return;
	}

	switch (_state) {
		case _State::FLY:
			_animatedSprite.PlaySpriteAnimation(_isOnGround ? "RedWingIdle" : "RedWingActive", { _position.x, _position.y - 8.0f });
			break;
		case _State::WALK:
			_animatedSprite.PlaySpriteAnimation(_variant == "red" ? "RedWalk" : "YellowWalk", _position);
			break;
		case _State::DIE:
			_animatedSprite.PlaySpriteAnimation(_animationName, _position, _scale);
			break;
	}
}

void Goomba::Release() {
	_animatedSprite.Release();
	_goombaTexture = nullptr;
}
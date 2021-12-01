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
	
	_variant = "red";
	_state = _State::WALK;

	animationName = "Die";
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
}

void Goomba::HandleStates() {
	_state = static_cast<_State>(_health);

	switch (_state) {
		case _State::FLY:
		case _State::WALK:
			_velocity.x = -_runSpeed * _normal.x;
			break;
		case _State::DIE:
			_velocity.x = 0.0f;
			_isOnGround = false;
			
			if (_scale.y != -1.0f) {
				_velocity.y = 0.0f;
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

	if (eventNormal.y == -1.0f) {
		_isOnGround = true;
	}

	switch (eventEntity->GetObjectType()) {
		case GameObjectType::GAMEOBJECT_TYPE_GOOMBA:
		case GameObjectType::GAMEOBJECT_TYPE_PARAGOOMBA:		
		case GameObjectType::GAMEOBJECT_TYPE_PIRANHAPLANT:
		case GameObjectType::GAMEOBJECT_TYPE_VENUSPLANT:
		case GameObjectType::GAMEOBJECT_TYPE_BOOMERANGBRO:
		case GameObjectType::GAMEOBJECT_TYPE_MOVINGPLATFORM:
		case GameObjectType::GAMEOBJECT_TYPE_QUESTIONBLOCK:
		case GameObjectType::GAMEOBJECT_TYPE_PBLOCK:
		case GameObjectType::GAMEOBJECT_TYPE_TILE:
			if (eventNormal.x != 0.0f) {
				_normal.x = -_normal.x;
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_KOOPA:
		case GameObjectType::GAMEOBJECT_TYPE_PARAKOOPA:
		case GameObjectType::GAMEOBJECT_TYPE_PLAYERFIREBALL:
		case GameObjectType::GAMEOBJECT_TYPE_TAIL:
			if (eventEntity->GetHealth() == 1) {
				animationName = "Walk";
			}
			else {
				if (eventNormal.x != 0.0f) {
					_normal.x = -_normal.x;
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_COIN:
			//Is brick
			if (eventEntity->GetHealth() == 3) {
				if (eventNormal.x != 0.0f) {
					_normal.x = -_normal.x;
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_SHINYBRICK:
			//Is not coin
			if (eventEntity->GetHealth() != 3) {
				if (eventNormal.x != 0.0f) {
					_normal.x = -_normal.x;
				}
			}
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
	switch (_state) {
		case _State::FLY:
			_animatedSprite.PlaySpriteAnimation(_isOnGround ? "WingIdle" : "WingActive", { _position.x, _position.y - 4.0f });
			break;
		case _State::WALK:
			_animatedSprite.PlaySpriteAnimation("Walk", _position);
			break;
		case _State::DIE:
			_animatedSprite.PlaySpriteAnimation(animationName, _position, _scale);
			break;
	}
}

void Goomba::Release() {
	_animatedSprite.Release();
	_goombaTexture = nullptr;
}
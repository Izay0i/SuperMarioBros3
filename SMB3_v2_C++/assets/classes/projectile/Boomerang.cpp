#include "../Entity.h"
#include "Boomerang.h"

Texture* Boomerang::_boomerangTexture = nullptr;

void Boomerang::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _boomerangTexture);
}

Boomerang::Boomerang() {
	_renderPriority = 1;
	_removeTime = 2000;

	_runSpeed = 0.1f;
	_velocity.x = _runSpeed;
	_velocity.y = -0.12f;
	_gravity = 0.0f;

	isPassThroughable = true;
	StartRemoveTimer();
}

Boomerang::~Boomerang() {}

void Boomerang::SetPosition(D3DXVECTOR2 position) {
	_position = position;
	_originalPos = position;
}

void Boomerang::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_boomerangTexture == nullptr) {
		_boomerangTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void Boomerang::HandleStates() {}

void Boomerang::HandleCollisionResult(
	LPCOLLISIONEVENT result, 
	D3DXVECTOR2& minTime, 
	D3DXVECTOR2& offset, 
	D3DXVECTOR2& normal, 
	D3DXVECTOR2& relativeDistance) {}

void Boomerang::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (IsRemoved() && GetTickCount64() - _removeStart > _removeTime) {
		_health = -1;
		_removeStart = 0;
	}

	if (!_isActive && _objectType >= GameObjectType::GAMEOBJECT_TYPE_GOOMBA) {
		return;
	}

	GameObject::Update(deltaTime);
	_position += _distance;
	
	if (_velocity.x > 0.0f) {
		_velocity.y += 0.0002f * deltaTime;
		
		if (_position.y >= _originalPos.y - 4.0f) {
			_velocity.x -= 0.0002f * deltaTime;
			if (_velocity.x <= _runSpeed) {
				_velocity.x = -_runSpeed;
			}
		}
	}
	else if (_velocity.x < 0.0f) {
		_velocity.y -= 0.0002f * deltaTime;

		if (_position.y <= _originalPos.y + 4.0f) {
			_velocity.x += 0.0002f * deltaTime;
			if (_velocity.x <= _runSpeed) {
				_velocity.x = -_runSpeed;
			}
		}
	}

	const float MAX_DISTANCE = 128.0f;
	if (_position.x - _originalPos.x >= MAX_DISTANCE && _velocity.x > 0.0f) {
		_velocity.y = 0.1f;
	}
	else if (_position.x <= _originalPos.x && _velocity.x < 0.0f) {
		_velocity.x = _runSpeed;
		_velocity.y = -0.1f;
	}
}

void Boomerang::Render() {
	_animatedSprite.PlaySpriteAnimation("Boomerang", _position);
}

void Boomerang::Release() {
	_animatedSprite.Release();
	_boomerangTexture = nullptr;
}
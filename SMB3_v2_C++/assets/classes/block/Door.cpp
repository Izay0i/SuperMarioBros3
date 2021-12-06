#include "../Entity.h"
#include "Door.h"
#include "../npc/MovingCeiling.h"

Texture* Door::_doorTexture = nullptr;
bool Door::canTriggerMovingCeiling;

void Door::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _doorTexture);
}

Door::Door() {
	_renderPriority = 1;
	_runSpeed = 0.02f;

	_idleTime = 500;

	isPassThroughable = true;
}

Door::~Door() {}

bool Door::IsIdling() const {
	return _idleStart != 0;
}

void Door::StartIdleTimer() {
	_idleStart = static_cast<DWORD>(GetTickCount64());
}

void Door::SetPosition(D3DXVECTOR2 position) {
	_position = position;
	_originalPos = position;
}

D3DXVECTOR2 Door::GetDestination() const {
	return _destination;
}

void Door::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_doorTexture == nullptr) {
		_doorTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);

	if (!_extraData.empty()) {
		_destination.x = std::stof(_extraData.at(0));
		_destination.y = std::stof(_extraData.at(1));
	}
}

void Door::HandleStates() {}

void Door::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void Door::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (canTriggerMovingCeiling) {
		_position.x = _originalPos.x;

		if (!IsIdling()) {
			if (_position.y < _originalPos.y) {
				StartIdleTimer();
				_position.y = _originalPos.y;
				_normal.y = 1.0f;
			}
			else if (_position.y > _originalPos.y + _MAX_HEIGHT) {
				StartIdleTimer();
				_position.y = _originalPos.y + _MAX_HEIGHT;
				_normal.y = -1.0f;
			}

			_velocity.y = _runSpeed * _normal.y;
		}
		else {
			_velocity.y = 0.0f;
		}
	}

	if (IsIdling() && GetTickCount64() - _idleStart > _idleTime) {
		_idleStart = 0;
	}

	Entity::Update(deltaTime);

	for (unsigned int i = 0; i < collidableEntities->size(); ++i) {
		Entity* entity = collidableEntities->at(i);
		if (entity->IsActive() && entity->GetHealth() > 0) {
			switch (entity->GetObjectType()) {
				case GameObjectType::GAMEOBJECT_TYPE_MOVINGCEILING:
				{
					MovingCeiling* movingCeiling = static_cast<MovingCeiling*>(entity);
					movingCeiling->isMoving = canTriggerMovingCeiling;
				}
				break;
			}
		}
	}
}

void Door::Render() {
	_animatedSprite.PlaySpriteAnimation("Top", { _position.x, _position.y - _hitbox.GetBoxHeight() });
	_animatedSprite.PlaySpriteAnimation("Bottom", _position);
}

void Door::Release() {
	_animatedSprite.Release();
	_doorTexture = nullptr;

	canTriggerMovingCeiling = false;
}
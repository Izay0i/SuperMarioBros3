#include "../Entity.h"
#include "Fireball.h"

Texture* Fireball::_fireballTexture = nullptr;

void Fireball::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _fireballTexture);
}

Fireball::Fireball() {
	_renderPriority = 2;
	_removeTime = 300;

	_travelSpeed = 0.0012f;
	_bounceSpeed = 0.2f;
	_gravity = 0.002f;

	_aliveTime = 8000;
	StartAliveTimer();
}

Fireball::~Fireball() {}

bool Fireball::IsAlive() const {
	return _aliveStart != 0;
}

void Fireball::StartAliveTimer() {
	_aliveStart = static_cast<DWORD>(GetTickCount64());
}

RECTF Fireball::GetBoundingBox(int index) const {
	return _health <= 0 ? RECTF() : GameObject::GetBoundingBox();
}

void Fireball::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_fireballTexture == nullptr) {
		_fireballTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);

	_runSpeed = _objectType == GameObjectType::GAMEOBJECT_TYPE_PFIREBALL ? 0.2f : 0.05f;
}

void Fireball::HandleStates() {
	_state = static_cast<_State>(_health);

	switch (_state) {
		case _State::BOUNCE:
			_velocity.x = _runSpeed * _normal.x;
			break;
		case _State::EXPLODE:
			_velocity = { 0, 0 };

			if (_health == 0 && !IsRemoved()) {
				StartRemoveTimer();
			}
			break;
	}
}

void Fireball::HandleCollisionResult(
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

	if (_objectType == GameObjectType::GAMEOBJECT_TYPE_PFIREBALL) {
		if (eventNormal.y == -1.0f) {
			_velocity.y = -_bounceSpeed;
		}
	}
	else if (_objectType == GameObjectType::GAMEOBJECT_TYPE_VFIREBALL) {
		minTime = { 1.0f, 1.0f };
		offset = normal = relativeDistance = { 0, 0 };
	}

	switch (_objectType) {
		case GameObjectType::GAMEOBJECT_TYPE_PFIREBALL:
			switch (eventEntity->GetObjectType()) {
				case GameObjectType::GAMEOBJECT_TYPE_VFIREBALL:
					TakeDamage();
					break;
				case GameObjectType::GAMEOBJECT_TYPE_QUESTIONBLOCK:
				case GameObjectType::GAMEOBJECT_TYPE_SHINYBRICK:
				case GameObjectType::GAMEOBJECT_TYPE_TILE:
					if (eventNormal.x != 0.0f) {
						TakeDamage();
					}
					break;
				//Ignore these entities
				case GameObjectType::GAMEOBJECT_TYPE_MARIO:
				case GameObjectType::GAMEOBJECT_TYPE_RMUSHROOM:
				case GameObjectType::GAMEOBJECT_TYPE_GMUSHROOM:
				case GameObjectType::GAMEOBJECT_TYPE_FLOWER:
				case GameObjectType::GAMEOBJECT_TYPE_STAR:
				case GameObjectType::GAMEOBJECT_TYPE_COIN:
				case GameObjectType::GAMEOBJECT_TYPE_BONUSITEM:
				case GameObjectType::GAMEOBJECT_TYPE_ONEWAYPLATFORM:
					//Pokemon Black & White, Black 2 & White 2 stories were the peak of the franchise, change my mind
					break;
				default:
					eventEntity->TakeDamage();
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_VFIREBALL:
			switch (eventEntity->GetObjectType()) {
				case GameObjectType::GAMEOBJECT_TYPE_MARIO:
					eventEntity->TakeDamage();
					break;
				case GameObjectType::GAMEOBJECT_TYPE_PFIREBALL:
					TakeDamage();
					break;
			}
			break;
	}
}

void Fireball::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (IsAlive() && GetTickCount64() - _aliveStart > _aliveTime) {
		_health = -1;
		_aliveStart = 0;
	}

	HandleStates();
	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);
	
	if (_objectType == GameObjectType::GAMEOBJECT_TYPE_VFIREBALL) {
		_velocity.y = -_travelSpeed * _normal.y * deltaTime;
	}
}

void Fireball::Render() {
	_animatedSprite.PlaySpriteAnimation("Fireball", _position);
}

void Fireball::Release() {
	_animatedSprite.Release();
	_fireballTexture = nullptr;
}
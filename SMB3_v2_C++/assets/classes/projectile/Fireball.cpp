#include "../Entity.h"
#include "Fireball.h"

LPDIRECT3DTEXTURE9 Fireball::_fireballTexture = nullptr;

void Fireball::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _fireballTexture);
}

Fireball::Fireball() {
	_renderPriority = 1;
	_removeTime = 300;

	_travelSpeed = 0.0012f;
	_runSpeed = _objectType == GameObjectType::GAMEOBJECT_TYPE_PFIREBALL ? 0.2f : 0.05f;
	_bounceSpeed = 0.2f;
	_gravity = 0.002f;

	_aliveTime = 10000;
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
	const LPDIRECT3DTEXTURE9& texture, 
	std::vector<std::string> extraData) 
{
	if (_fireballTexture == nullptr) {
		_fireballTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void Fireball::HandleStates() {
	switch (_health) {
		case 0:
			_state = _State::EXPLODE;
			break;
		case 1:
			_state = _State::BOUNCE;
			break;
	}

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

	if (eventNormal.y == -1.0f) {
		if (_objectType == GameObjectType::GAMEOBJECT_TYPE_PFIREBALL) {
			_velocity.y = -_bounceSpeed;
		}
	}

	switch (_objectType) {
		case GameObjectType::GAMEOBJECT_TYPE_PFIREBALL:
			switch (eventEntity->GetObjectType()) {
				case GameObjectType::GAMEOBJECT_TYPE_TILE:
					if (eventNormal.x != 0.0f) {
						TakeDamage();
					}
					break;
				//Ignore these entities
				case GameObjectType::GAMEOBJECT_TYPE_RMUSHROOM:
				case GameObjectType::GAMEOBJECT_TYPE_GMUSHROOM:
				case GameObjectType::GAMEOBJECT_TYPE_FLOWER:
				case GameObjectType::GAMEOBJECT_TYPE_STAR:
				case GameObjectType::GAMEOBJECT_TYPE_COIN:
				case GameObjectType::GAMEOBJECT_TYPE_BONUSITEM:
				case GameObjectType::GAMEOBJECT_TYPE_QUESTIONBLOCK:
				case GameObjectType::GAMEOBJECT_TYPE_SHINYBRICK:
				case GameObjectType::GAMEOBJECT_TYPE_ONEWAYPLATFORM:
					//NULL
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
	
	switch (_objectType) {
		case GameObjectType::GAMEOBJECT_TYPE_PFIREBALL:
			_velocity.y = -_travelSpeed * _normal.y * deltaTime;
			break;
		case GameObjectType::GAMEOBJECT_TYPE_VFIREBALL:
			//_velocity.y += _gravity * deltaTime;
			break;
	}
	
	if (_objectType == GameObjectType::GAMEOBJECT_TYPE_VFIREBALL) {
		_position += _distance;
	}
}

void Fireball::Render() {
	if (!_isActive) {
		return;
	}

	switch (_objectType) {
		case GameObjectType::GAMEOBJECT_TYPE_PFIREBALL:
			_animatedSprite.PlaySpriteAnimation("PFireball", _position);
			break;
		case GameObjectType::GAMEOBJECT_TYPE_VFIREBALL:
			_animatedSprite.PlaySpriteAnimation("VFireball", _position);
			break;
	}
}

void Fireball::Release() {
	_fireballTexture = nullptr;
}
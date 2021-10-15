#include "../Entity.h"
#include "Mushroom.h"

Texture* Mushroom::_shroomTexture = nullptr;

void Mushroom::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _shroomTexture);
}

Mushroom::Mushroom() {
	_renderPriority = 2;
	_removeTime = 50;

	_runSpeed = 0.002f;
	_jumpSpeed = 0.2f;
	_gravity = 0.002f;

	_emergeTime = 700;

	isPassThroughable = true;
	StartEmergeTimer();
}

Mushroom::~Mushroom() {}

bool Mushroom::IsEmerging() const {
	return _emergeStart != 0;
}

void Mushroom::StartEmergeTimer() {
	_emergeStart = static_cast<DWORD>(GetTickCount64());
}

RECTF Mushroom::GetBoundingBox(int index) const {
	return _health <= 0 ? RECTF() : GameObject::GetBoundingBox();
}

void Mushroom::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_shroomTexture == nullptr) {
		_shroomTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void Mushroom::HandleStates() {
	if (_health == 0 && !IsRemoved()) {
		StartRemoveTimer();
	}
}

void Mushroom::HandleCollisionResult(
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

	switch (eventEntity->GetObjectType()) {
		case GameObjectType::GAMEOBJECT_TYPE_RMUSHROOM:
		case GameObjectType::GAMEOBJECT_TYPE_GMUSHROOM:
		case GameObjectType::GAMEOBJECT_TYPE_LEAF:
		case GameObjectType::GAMEOBJECT_TYPE_COIN:
			minTime = { 1.0f, 1.0f };
			offset = normal = relativeDistance = { 0, 0 };
			break;
		case GameObjectType::GAMEOBJECT_TYPE_TILE:
			if (eventNormal.x != 0.0f) {
				_normal.x = -_normal.x;
			}
			break;
	}
}

void Mushroom::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (IsEmerging() && GetTickCount64() - _emergeStart > _emergeTime) {
		_renderPriority = 1;
		_emergeStart = 0;
	}

	HandleStates();
	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);

	if (IsEmerging()) {
		_velocity.y -= 0.00005f * deltaTime;
	}
	else {
		_velocity.x = -_runSpeed * _normal.x * deltaTime;
	}
}

void Mushroom::Render() {
	switch (_objectType) {
		case GameObjectType::GAMEOBJECT_TYPE_RMUSHROOM:
			_animatedSprite.PlaySpriteAnimation("RedShroom", _position);
			break;
		case GameObjectType::GAMEOBJECT_TYPE_GMUSHROOM:
			_animatedSprite.PlaySpriteAnimation("GreenShroom", _position);
			break;
	}
}

void Mushroom::Release() {
	_animatedSprite.Release();
	_shroomTexture = nullptr;
}
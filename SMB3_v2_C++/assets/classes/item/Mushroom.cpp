#include "../Entity.h"
#include "Mushroom.h"

Texture* Mushroom::_shroomTexture = nullptr;

void Mushroom::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _shroomTexture);
}

Mushroom::Mushroom() {
	_renderPriority = 4;
	_removeTime = 50;

	_runSpeed = 0.06f;
	_jumpSpeed = 0.018f;
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

	switch (eventEntity->GetObjectType()) {
		case GameObjectType::GAMEOBJECT_TYPE_COIN:
			//Is brick
			if (eventEntity->GetHealth() == 3 && eventNormal.x != 0.0f) {
				_normal.x = -_normal.x;
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_SHINYBRICK:
			//Is not coim
			if (eventEntity->GetHealth() != 3 && eventNormal.x != 0.0f) {
				_normal.x = -_normal.x;
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_REDMUSHROOM:
		case GameObjectType::GAMEOBJECT_TYPE_GREENMUSHROOM:
		case GameObjectType::GAMEOBJECT_TYPE_FLOWER:
		case GameObjectType::GAMEOBJECT_TYPE_QUESTIONBLOCK:
		case GameObjectType::GAMEOBJECT_TYPE_PBLOCK:
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
		_renderPriority = 2;
		_emergeStart = 0;
	}

	if (IsEmerging()) {
		_velocity.y = -_jumpSpeed;
	}
	else {
		_velocity.x = -_runSpeed * _normal.x;
	}

	HandleStates();
	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);
}

void Mushroom::Render() {
	_animatedSprite.PlaySpriteAnimation("Shroom", _position, _scale);
}

void Mushroom::Release() {
	_animatedSprite.Release();
	_shroomTexture = nullptr;
}
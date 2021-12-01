#include "../Entity.h"
#include "Podoboo.h"

Texture* Podoboo::_podTexture = nullptr;

void Podoboo::_ParseSprites(std::string)
{
}

Podoboo::Podoboo() {
	_renderPriority = 1;

	_jumpSpeed = 0.22f;
	_gravity = 0.22f;

	_inactiveTime = 2000;

	isPassThroughable = true;
}

Podoboo::~Podoboo() {}

bool Podoboo::IsInactive() const {
	return _inactiveStart != 0;
}

void Podoboo::StartInactiveTimer() {
	_inactiveStart = static_cast<DWORD>(GetTickCount64());
}

RECTF Podoboo::GetBoundingBox(int) const {
	return IsInactive() ? RECTF() : GameObject::GetBoundingBox();
}

void Podoboo::SetPosition(D3DXVECTOR2 position) {
	_position = position;
	_originalPos = position;
}

void Podoboo::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_podTexture == nullptr) {
		_podTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void Podoboo::TakeDamage() {}

void Podoboo::HandleStates() {}

void Podoboo::HandleCollisionResult(
	LPCOLLISIONEVENT result, 
	D3DXVECTOR2& minTime, 
	D3DXVECTOR2& offset, 
	D3DXVECTOR2& normal, 
	D3DXVECTOR2& relativeDistance) 
{
	Entity* eventEntity = result->entity;
	D3DXVECTOR2 eventNormal = result->normal;

	switch (eventEntity->GetObjectType()) {
		case GameObjectType::GAMEOBJECT_TYPE_LAVAPOOL:
			StartInactiveTimer();
			break;
	}
}

void Podoboo::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (_position.y <= _originalPos.y + _MAX_HEIGHT) {
		_scale.y = -1.0f;
	}
	
	if (IsInactive()) {
		_gravity = 0.0f;
	}

	if (IsInactive() && GetTickCount64() - _inactiveStart > _inactiveTime) {
		_scale.y = 1.0f;
		_gravity = 0.22f;
		_velocity.y = -_jumpSpeed;
		_inactiveStart = 0;
	}

	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);
}

void Podoboo::Render() {
	if (IsInactive()) {
		_animatedSprite.PlaySpriteAnimation("Splash", _position);
	}
	else {
		_animatedSprite.PlaySpriteAnimation("Podoboo", _position, _scale);
	}
}

void Podoboo::Release() {
	_animatedSprite.Release();
	_podTexture = nullptr;
}
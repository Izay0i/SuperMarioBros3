#include "../Entity.h"
#include "BuzzyBeetle.h"

Texture* BuzzyBeetle::_buzzyTexture = nullptr;

void BuzzyBeetle::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _buzzyTexture);
}

BuzzyBeetle::BuzzyBeetle() {
	_renderPriority = 1;

	_bounceSpeed = 0.3f;

	_gravity = 0.002f;
}

BuzzyBeetle::~BuzzyBeetle() {}

RECTF BuzzyBeetle::GetBoundingBox(int) const {
	return _health <= 0 ? RECTF() : GameObject::GetBoundingBox();
}

void BuzzyBeetle::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_buzzyTexture == nullptr) {
		_buzzyTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void BuzzyBeetle::HandleStates() {}

void BuzzyBeetle::HandleCollisionResult(
	LPCOLLISIONEVENT result, 
	D3DXVECTOR2& minTime, 
	D3DXVECTOR2& offset, 
	D3DXVECTOR2& normal, 
	D3DXVECTOR2& relativeDistance) 
{
	Entity* eventEntity = result->entity;
	D3DXVECTOR2 eventNormal = result->normal;

	switch (eventEntity->GetObjectType()) {
		case GameObjectType::GAMEOBJECT_TYPE_PROPKOOPASHELL:
			if (eventNormal.x != 0.0f) {
				_health = 0;
				_scale.y = -1.0f;
				_velocity.y = -_bounceSpeed;
				_runSpeed = 0.08f;
				_gravity = 0.0014f;
			}
			break;
	}
}

void BuzzyBeetle::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	_velocity.x = _runSpeed;
	Entity::Update(deltaTime, collidableEntities, collidableTiles);
}

void BuzzyBeetle::Render() {
	_animatedSprite.PlaySpriteAnimation("Idle", _position, _scale);
}

void BuzzyBeetle::Release() {
	_animatedSprite.Release();
	_buzzyTexture = nullptr;
}
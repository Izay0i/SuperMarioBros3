#include "../Entity.h"
#include "Star.h"

Texture* Star::_starTexture = nullptr;

void Star::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _starTexture);
}

Star::Star() {
	_renderPriority = 2;
	_removeTime = 50;

	_bounceSpeed = 0.5f;
	_gravity = 0.002f;

	isPassThroughable = true;
}

Star::~Star() {}

void Star::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_starTexture == nullptr) {
		_starTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void Star::HandleStates() {
	_velocity.x = _runSpeed;
}

void Star::HandleCollisionResult(
	LPCOLLISIONEVENT result, 
	D3DXVECTOR2& minTime, 
	D3DXVECTOR2& offset, 
	D3DXVECTOR2& normal, 
	D3DXVECTOR2& relativeDistance) 
{
	Entity* eventEntity = result->entity;
	D3DXVECTOR2 eventNormal = result->normal;

	if (eventNormal.y == -1.0f) {
		_velocity.y = -_bounceSpeed;
		_runSpeed = 0.08f;
	}
}

void Star::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	HandleStates();
	Entity::Update(deltaTime, nullptr, collidableTiles);
}

void Star::Render() {
	_animatedSprite.PlaySpriteAnimation("Star", _position, _scale);
}

void Star::Release() {
	_animatedSprite.Release();
	_starTexture = nullptr;
}
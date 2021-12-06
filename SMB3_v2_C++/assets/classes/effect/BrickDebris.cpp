#include "../Entity.h"
#include "BrickDebris.h"

Texture* BrickDebris::_debrisTexture = nullptr;

void BrickDebris::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _debrisTexture);
}

BrickDebris::BrickDebris() {
	_renderPriority = 1;
	_health = 0;
	_gravity = 0.001f;
	_removeTime = 1000;

	isPassThroughable = true;
}

BrickDebris::~BrickDebris() {}

void BrickDebris::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_debrisTexture == nullptr) {
		_debrisTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void BrickDebris::HandleStates() {}

void BrickDebris::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void BrickDebris::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	Entity::Update(deltaTime);
}

void BrickDebris::Render() {
	_animatedSprite.PlaySpriteAnimation("Brick", _position, _scale);
}

void BrickDebris::Release() {
	_animatedSprite.Release();
	_debrisTexture = nullptr;
}
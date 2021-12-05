#include "../Entity.h"
#include "Orb.h"

Texture* Orb::_orbTexture = nullptr;

void Orb::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _orbTexture);
}

Orb::Orb() {
	_renderPriority = 2;
	_gravity = 0.22f;

	isPassThroughable = true;

	const float BOUNCE_VALUE = 0.16f;
	_velocity.y = -BOUNCE_VALUE;
}

Orb::~Orb() {}

void Orb::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_orbTexture == nullptr) {
		_orbTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void Orb::HandleStates() {}

void Orb::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void Orb::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);
}

void Orb::Render() {
	_animatedSprite.PlaySpriteAnimation("Orb", _position);
}

void Orb::Release() {
	_animatedSprite.Release();
	_orbTexture = nullptr;
}
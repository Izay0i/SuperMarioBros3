#include "../Entity.h"
#include "OrbEffect.h"

Texture* OrbEffect::_orbTexture = nullptr;

void OrbEffect::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _orbTexture);
}

OrbEffect::OrbEffect() {
	_normal = { 0.0f, 0.0f };

	_renderPriority = 1;
	_health = 0;
	_runSpeed = 0.04f;
	_removeTime = 900;

	isPassThroughable = true;
}

OrbEffect::~OrbEffect() {}

void OrbEffect::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_orbTexture == nullptr) {
		_orbTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void OrbEffect::HandleStates() {}

void OrbEffect::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void OrbEffect::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	_velocity.x = _runSpeed * _normal.x;
	_velocity.y = _runSpeed * _normal.y;
	Entity::Update(deltaTime);
}

void OrbEffect::Render() {
	_animatedSprite.PlaySpriteAnimation("Star", _position);
}

void OrbEffect::Release() {
	_animatedSprite.Release();
	_orbTexture = nullptr;
}
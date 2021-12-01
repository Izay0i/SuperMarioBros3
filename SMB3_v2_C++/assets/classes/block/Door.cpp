#include "../Entity.h"
#include "Door.h"

Texture* Door::_doorTexture = nullptr;

void Door::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _doorTexture);
}

Door::Door() {
	_renderPriority = 1;

	_runSpeed = 0.22f;

	isPassThroughable = true;
}

Door::~Door() {}

void Door::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_doorTexture == nullptr) {
		_doorTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);

	if (!_extraData.empty()) {
		_destination.x = std::stof(_extraData.at(0));
		_destination.y = std::stof(_extraData.at(1));
	}
}

void Door::TakeDamage() {}

void Door::HandleStates() {}

void Door::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void Door::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);
}

void Door::Render() {
	_animatedSprite.PlaySpriteAnimation("Top", { _position.x, _position.y - _hitbox.GetBoxHeight() });
	_animatedSprite.PlaySpriteAnimation("Bottom", _position);
}

void Door::Release() {
	_animatedSprite.Release();
	_doorTexture = nullptr;
}
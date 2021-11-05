#include "../Entity.h"
#include "PropNormalKoopa.h"

Texture* PropNormalKoopa::_normalTexture = nullptr;

void PropNormalKoopa::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _normalTexture);
}

PropNormalKoopa::PropNormalKoopa() {
	_renderPriority = 1;
	_runSpeed = 0.03f;

	isPassThroughable = true;
}

PropNormalKoopa::~PropNormalKoopa() {}

RECTF PropNormalKoopa::GetBoundingBox(int) const {
	return RECTF();
}

void PropNormalKoopa::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_normalTexture == nullptr) {
		_normalTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void PropNormalKoopa::HandleStates() {}

void PropNormalKoopa::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void PropNormalKoopa::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	_scale.x = -1.0f;
	_velocity.x = _runSpeed * _normal.x;
	Entity::Update(deltaTime);
}

void PropNormalKoopa::Render() {
	_animatedSprite.PlaySpriteAnimation("Walk", _position, _scale);
}

void PropNormalKoopa::Release() {
	_animatedSprite.Release();
	_normalTexture = nullptr;
}
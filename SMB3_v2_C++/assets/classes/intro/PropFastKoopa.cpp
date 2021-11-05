#include "../Entity.h"
#include "PropFastKoopa.h"

Texture* PropFastKoopa::_fastKoopaTexture = nullptr;

void PropFastKoopa::_ParseSprites(std::string line){
	_animatedSprite.ParseSprites(line, _fastKoopaTexture);
}

PropFastKoopa::PropFastKoopa(){
	_renderPriority = 1;
	_runSpeed = 0.08f;

	isPassThroughable = true;
}

PropFastKoopa::~PropFastKoopa() {}

RECTF PropFastKoopa::GetBoundingBox(int) const {
	return RECTF();
}

void PropFastKoopa::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_fastKoopaTexture == nullptr) {
		_fastKoopaTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void PropFastKoopa::HandleStates() {}

void PropFastKoopa::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void PropFastKoopa::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* ocllidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	_scale.x = -1.0f;
	_velocity.x = _runSpeed * _normal.x;
	Entity::Update(deltaTime);
}

void PropFastKoopa::Render() {
	_animatedSprite.PlaySpriteAnimation("Walk", _position, _scale);
}

void PropFastKoopa::Release() {
	_animatedSprite.Release();
	_fastKoopaTexture = nullptr;
}
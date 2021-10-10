#include "../Entity.h"
#include "BuzzyBeetle.h"

Texture* BuzzyBeetle::_buzzyTexture = nullptr;

void BuzzyBeetle::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _buzzyTexture);
}

BuzzyBeetle::BuzzyBeetle() {

}

BuzzyBeetle::~BuzzyBeetle() {}

RECTF BuzzyBeetle::GetBoundingBox(int) const {
	return RECTF();
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

void BuzzyBeetle::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void BuzzyBeetle::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	
}

void BuzzyBeetle::Render() {
	_animatedSprite.PlaySpriteAnimation("Idle", _position, _scale);
}

void BuzzyBeetle::Release() {
	_animatedSprite.Release();
	_buzzyTexture = nullptr;
}
#include "../Entity.h"
#include "PropPlant.h"

Texture* PropPlant::_propTexture = nullptr;

void PropPlant::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _propTexture);
}

PropPlant::PropPlant() {
	_renderPriority = 3;
}

PropPlant::~PropPlant() {}

RECTF PropPlant::GetBoundingBox(int) const {
	return RECTF();
}

void PropPlant::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_propTexture == nullptr) {
		_propTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void PropPlant::HandleStates() {}

void PropPlant::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void PropPlant::Update(DWORD, std::vector<Entity*>*, std::vector<Entity*>*, Grid*) {}

void PropPlant::Render() {
	_animatedSprite.PlaySpriteAnimation("Plant", _position, _scale);
}

void PropPlant::Release() {
	_animatedSprite.Release();
	_propTexture = nullptr;
}
#include "../Entity.h"
#include "SelectText.h"

Texture* SelectText::_selectTexture = nullptr;

void SelectText::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _selectTexture);
}

SelectText::SelectText() {
	_renderPriority = 10;
}

SelectText::~SelectText() {}

RECTF SelectText::GetBoundingBox(int) const {
	return RECTF();
}

void SelectText::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_selectTexture == nullptr) {
		_selectTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void SelectText::HandleStates() {

}

void SelectText::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void SelectText::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{

}

void SelectText::Render() {
	_animatedSprite.PlaySpriteAnimation(isMultiplayer ? "2P" : "1P", _position, _scale);
}

void SelectText::Release() {
	_animatedSprite.Release();
	_selectTexture = nullptr;
}
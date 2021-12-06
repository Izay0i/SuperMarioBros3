#include "../Entity.h"
#include "MaskTile.h"

Texture* MaskTile::_maskTexture = nullptr;

void MaskTile::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _maskTexture);
}

MaskTile::MaskTile() {
	_renderPriority = -1;
}

MaskTile::~MaskTile() {}

void MaskTile::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_maskTexture == nullptr) {
		_maskTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void MaskTile::HandleStates() {}

void MaskTile::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void MaskTile::Update(DWORD, std::vector<Entity*>*, std::vector<Entity*>*, Grid*) {}

void MaskTile::Render() {
	_animatedSprite.PlaySpriteAnimation("Mask", _position);
}

void MaskTile::Release() {
	_animatedSprite.Release();
	_maskTexture = nullptr;
}
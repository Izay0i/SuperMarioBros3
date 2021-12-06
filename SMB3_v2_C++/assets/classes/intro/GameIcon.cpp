#include "../Entity.h"
#include "GameIcon.h"

Texture* GameIcon::_iconTexture = nullptr;

void GameIcon::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _iconTexture);
}

GameIcon::GameIcon() {
	_renderPriority = 11;
}

GameIcon::~GameIcon() {}

RECTF GameIcon::GetBoundingBox(int) const {
	return RECTF();
}

void GameIcon::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_iconTexture == nullptr) {
		_iconTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void GameIcon::HandleStates() {}

void GameIcon::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void GameIcon::Update(DWORD, std::vector<Entity*>*, std::vector<Entity*>*, Grid*) {}

void GameIcon::Render() {
	_animatedSprite.PlaySpriteAnimation("Icon", _position, _scale);
}

void GameIcon::Release() {
	_animatedSprite.Release();
	_iconTexture = nullptr;
}
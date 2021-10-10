#include "../Entity.h"
#include "GameLogo.h"

Texture* GameLogo::_logoTexture = nullptr;

void GameLogo::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _logoTexture);
}

RECTF GameLogo::GetBoundingBox(int) const {
	return RECTF();
}

void GameLogo::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_logoTexture == nullptr) {
		_logoTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void GameLogo::HandleStates() {}

void GameLogo::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void GameLogo::Update(DWORD, std::vector<Entity*>*, std::vector<Entity*>*, Grid*) {}

void GameLogo::Render() {
	_animatedSprite.PlaySpriteAnimation("Logo", _position);
}

void GameLogo::Release() {
	_animatedSprite.Release();
	_logoTexture = nullptr;
}
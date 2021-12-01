#include "../Entity.h"
#include "LavaPool.h"

Texture* LavaPool::_lavaTexture = nullptr;

void LavaPool::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _lavaTexture);
}

LavaPool::LavaPool() {
	_renderPriority = 2;

	isPassThroughable = true;
}

LavaPool::~LavaPool() {}

void LavaPool::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_lavaTexture == nullptr) {
		_lavaTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void LavaPool::HandleStates() {}

void LavaPool::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void LavaPool::Update(DWORD, std::vector<Entity*>*, std::vector<Entity*>*, Grid*) {}

void LavaPool::Render() {
	_animatedSprite.PlaySpriteAnimation("LavaPool", _position, _scale);
}

void LavaPool::Release() {
	_animatedSprite.Release();
	_lavaTexture = nullptr;
}
#include "../Entity.h"
#include "Cactus.h"

Texture* Cactus::_cactusTexture = nullptr;

void Cactus::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _cactusTexture);
}

Cactus::Cactus() {
	_renderPriority = 1;
}

Cactus::~Cactus() {}

void Cactus::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_cactusTexture == nullptr) {
		_cactusTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void Cactus::HandleStates() {}

void Cactus::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void Cactus::Update(DWORD, std::vector<Entity*>*, std::vector<Entity*>*, Grid*) {}

void Cactus::Render() {
	_animatedSprite.PlaySpriteAnimation("Cactus", _position);
}

void Cactus::Release() {
	_animatedSprite.Release();
	_cactusTexture = nullptr;
}
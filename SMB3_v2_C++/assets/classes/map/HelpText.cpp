#include "../Entity.h"
#include "HelpText.h"

Texture* HelpText::_helpTexture = nullptr;

void HelpText::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _helpTexture);
}

void HelpText::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_helpTexture == nullptr) {
		_helpTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void HelpText::HandleStates() {}

void HelpText::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void HelpText::Update(DWORD, std::vector<Entity*>*, std::vector<Entity*>*, Grid*) {}

void HelpText::Render() {
	_animatedSprite.PlaySpriteAnimation("Help", _position);
}

void HelpText::Release() {
	_animatedSprite.Release();
	_helpTexture = nullptr;
}
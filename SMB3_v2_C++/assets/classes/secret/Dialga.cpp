#include "../Entity.h"
#include "Dialga.h"
#include "../audio/AudioService.h"

Texture* Dialga::_dialgaTexture = nullptr;

void Dialga::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _dialgaTexture);
}

Dialga::Dialga() {
	isPassThroughable = true;
	AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_TIME);
}

Dialga::~Dialga() {}

void Dialga::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_dialgaTexture == nullptr) {
		_dialgaTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void Dialga::HandleStates() {}

void Dialga::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void Dialga::Update(DWORD, std::vector<Entity*>*, std::vector<Entity*>*, Grid*) {}

void Dialga::Render() {
	_animatedSprite.PlaySpriteAnimation("Dialga", _position);
}

void Dialga::Release() {
	_animatedSprite.Release();
	_dialgaTexture = nullptr;
}
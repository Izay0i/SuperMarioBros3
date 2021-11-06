#include "../Entity.h"
#include "Palkia.h"
#include "../audio/AudioService.h"

Texture* Palkia::_palkiaTexture = nullptr;

void Palkia::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _palkiaTexture);
}

Palkia::Palkia() {
	AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_SPACE);
}

Palkia::~Palkia() {}

void Palkia::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_palkiaTexture == nullptr) {
		_palkiaTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void Palkia::HandleStates() {}

void Palkia::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void Palkia::Update(DWORD, std::vector<Entity*>*, std::vector<Entity*>*, Grid*) {}

void Palkia::Render(){
	_animatedSprite.PlaySpriteAnimation("Palkia", _position);
}

void Palkia::Release() {
	_animatedSprite.Release();
	_palkiaTexture = nullptr;
}
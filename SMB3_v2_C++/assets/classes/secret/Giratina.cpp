#include "../Entity.h"
#include "Giratina.h"
#include "../audio/AudioService.h"

Texture* Giratina::_giratinaTexture = nullptr;

void Giratina::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _giratinaTexture);
}

Giratina::Giratina() {
	_renderPriority = 1;

	_runSpeed = 0.18f;
	_transitionTime = 500;

	isPassThroughable = true;

	StartTransitionTimer();
}

Giratina::~Giratina() {}

bool Giratina::IsInTransition() const {
	return _transitionStart != 0;
}

void Giratina::StartTransitionTimer() {
	_transitionStart = static_cast<DWORD>(GetTickCount64());
}

void Giratina::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_giratinaTexture == nullptr) {
		_giratinaTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void Giratina::HandleStates() {}

void Giratina::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void Giratina::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (IsInTransition()) {
		_velocity.y = _runSpeed;
	}

	if (IsInTransition() && GetTickCount64() - _transitionStart > _transitionTime) {
		_velocity.y = 0.0f;
		_transitionStart = 0;

		AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_ANTIMATTER);
	}

	Entity::Update(deltaTime);
}

void Giratina::Render() {
	_animatedSprite.PlaySpriteAnimation(IsInTransition() ? "Static" : "Cry", _position);
}

void Giratina::Release() {
	_animatedSprite.Release();
	_giratinaTexture = nullptr;
}
#include "../Entity.h"
#include "Curtain.h"

Texture* Curtain::_curtainTexture = nullptr;

void Curtain::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _curtainTexture);
}

Curtain::Curtain() {
	_renderPriority = 1;
	_curlUpTime = 5000;

	StartCurlUpTimer();
}

Curtain::~Curtain() {}

bool Curtain::IsCurlingUp() const {
	return _curlUpStart != 0;
}

void Curtain::StartCurlUpTimer() {
	_curlUpStart = static_cast<DWORD>(GetTickCount64());
}

RECTF Curtain::GetBoundingBox(int) const {
	return RECTF();
}

void Curtain::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_curtainTexture == nullptr) {
		_curtainTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void Curtain::HandleStates() {}

void Curtain::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void Curtain::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	const unsigned int TIME_TO_CURL_UP = 1000;
	if (IsCurlingUp() && GetTickCount64() - _curlUpStart > TIME_TO_CURL_UP) {
		_velocity.y = -0.05f;
	}

	if (IsCurlingUp() && GetTickCount64() - _curlUpStart > _curlUpTime) {
		_curlUpStart = 0;
		_velocity = { 0.0f, 0.0f };
		_scale = { 0.0f, 0.0f };
		_position.y = 2.0f;
	}

	Entity::Update(deltaTime);
}

void Curtain::Render() {
	_animatedSprite.PlaySpriteAnimation(IsCurlingUp() ? "Default" : "Curled", _position, _scale);
}

void Curtain::Release() {
	_animatedSprite.Release();
	_curtainTexture = nullptr;
}
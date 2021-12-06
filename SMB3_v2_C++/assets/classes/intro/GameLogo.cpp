#include "../Entity.h"
#include "GameLogo.h"

Texture* GameLogo::_logoTexture = nullptr;

GameLogo::GameLogo() {
	_renderPriority = 11;
	_fallDownTime = 400;
}

GameLogo::~GameLogo() {}

bool GameLogo::IsFallingDown() const {
	return _fallDownStart != 0;
}

void GameLogo::StartFallDownTimer() {
	_fallDownStart = static_cast<DWORD>(GetTickCount64());
}

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

void GameLogo::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (IsFallingDown()) {
		_velocity.y = 0.2f;
	}

	if (IsFallingDown() && GetTickCount64() - _fallDownStart > _fallDownTime) {
		_fallDownStart = 0;
		_velocity.y = 0.0f;
		_position.y = 60.0f;
	}

	Entity::Update(deltaTime);
}

void GameLogo::Render() {
	_animatedSprite.PlaySpriteAnimation("Logo", _position);
}

void GameLogo::Release() {
	_animatedSprite.Release();
	_logoTexture = nullptr;
}
#include "../Entity.h"
#include "HammerBro.h"

Texture* HammerBro::_broTexture = nullptr;

void HammerBro::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _broTexture);
}

HammerBro::HammerBro() {
	_renderPriority = 1;
}

HammerBro::~HammerBro() {}

RECTF HammerBro::GetBoundingBox(int) const {
	return RECTF();
}

void HammerBro::SetPosition(D3DXVECTOR2 position) {
	_position = position;
	_originalPos = position;
}

void HammerBro::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_broTexture == nullptr) {
		_broTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void HammerBro::HandleStates() {}

void HammerBro::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void HammerBro::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	const float X_OFFSET = 8.0f;
	_velocity.x = 0.02f * _normal.x;
	Entity::Update(deltaTime);

	if (_position.x <= _originalPos.x - X_OFFSET) {
		_normal.x = 1.0f;
	}
	else if (_position.x >= _originalPos.x + X_OFFSET) {
		_normal.x = -1.0f;
	}
	_scale.x = _normal.x;
}

void HammerBro::Render() {
	_animatedSprite.PlaySpriteAnimation("HammerBro", _position, _scale);
}

void HammerBro::Release() {
	_animatedSprite.Release();
	_broTexture = nullptr;
}
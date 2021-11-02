#include "../Entity.h"
#include "Coin.h"

Texture* Coin::_coinTexture = nullptr;

void Coin::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _coinTexture);
}

Coin::Coin() {
	_renderPriority = 2;
	_removeTime = 50;

	_gravity = 0.0005f;

	_popUpTime = 700;

	isPassThroughable = true;
}

Coin::~Coin() {}

bool Coin::IsPoppedUp() const {
	return _popUpStart != 0;
}

void Coin::StartPopUpTimer() {
	_popUpStart = static_cast<DWORD>(GetTickCount64());
}

RECTF Coin::GetBoundingBox(int index) const {
	return _health <= 0 || _health == 2 ? RECTF() : GameObject::GetBoundingBox();
}

void Coin::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_coinTexture == nullptr) {
		_coinTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void Coin::TakeDamage() {
	if (_health > 0 && _state != _State::BRICK) {
		--_health;
		tookDamage = true;
	}
}

void Coin::HandleStates() {
	_state = static_cast<_State>(_health);
	isPassThroughable = _state != _State::BRICK;

	if (_health == 0 && !IsRemoved()) {
		StartRemoveTimer();
	}
}

void Coin::HandleCollisionResult(
	LPCOLLISIONEVENT result, 
	D3DXVECTOR2& minTime, 
	D3DXVECTOR2& offset, 
	D3DXVECTOR2& normal, 
	D3DXVECTOR2& relativeDistance) {}

void Coin::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (IsRemoved() && GetTickCount64() - _removeStart > _removeTime) {
		_health = -1;
		_removeStart = 0;
	}
	
	if (IsPoppedUp() && GetTickCount64() - _popUpStart > _popUpTime) {
		_popUpStart = 0;

		tookDamage = true;
		_health = -1;
	}

	HandleStates();
	if (_state == _State::PUSHEDFROMBLOCK) {
		Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);
		_position += _distance;
	}
}

void Coin::Render() {
	switch (_state) {
		case _State::ROTATE:
			_animatedSprite.PlaySpriteAnimation("Coin", _position);
			break;
		case _State::PUSHEDFROMBLOCK:
			_animatedSprite.PlaySpriteAnimation("Pushed", _position);
			break;
		case _State::BRICK:
			_animatedSprite.PlaySpriteAnimation("Brick", _position);
			break;
	}
}

void Coin::Release() {
	_animatedSprite.Release();
	_coinTexture = nullptr;
}
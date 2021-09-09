#include "../Entity.h"
#include "Coin.h"

LPDIRECT3DTEXTURE9 Coin::_coinTexture = nullptr;

void Coin::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _coinTexture);
}

Coin::Coin() {
	_renderPriority = 2;
	_removeTime = 50;

	_gravity = 0.0005f;
}

Coin::~Coin() {}

RECTF Coin::GetBoundingBox(int index) const {
	return _health <= 0 ? RECTF() : GameObject::GetBoundingBox();
}

void Coin::ParseData(
	std::string dataPath, 
	const LPDIRECT3DTEXTURE9& texture, 
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

	if (_health == 0 && !IsRemoved()) {
		StartRemoveTimer();
	}
}

void Coin::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void Coin::Update(DWORD deltaTime, std::vector<Entity*>* collidableEntities, std::vector<Entity*>* collidableTiles, Grid* grid) {
	if (IsRemoved() && GetTickCount64() - _removeStart > _removeTime) {
		_health = -1;
		_removeStart = 0;
	}

	if (!_isActive && _objectType >= GameObjectType::GAMEOBJECT_TYPE_GOOMBA) {
		return;
	}
	
	HandleStates();
	if (_state == _State::PUSHEDFROMBLOCK) {
		GameObject::Update(deltaTime);
		_velocity.y += _gravity * deltaTime;
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

			break;
	}
}

void Coin::Release() {
	_coinTexture = nullptr;
}
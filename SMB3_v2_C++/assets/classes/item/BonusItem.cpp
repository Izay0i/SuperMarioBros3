#include "../Entity.h"
#include "BonusItem.h"

#include <random>

Texture* BonusItem::_bonusTexture = nullptr;

void BonusItem::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _bonusTexture);
}

BonusItem::BonusItem() {
	_renderPriority = 2;
	_removeTime = 3000;
	_health = 2;

	_possibleItems = {
		GameObjectType::GAMEOBJECT_TYPE_STAR,
		GameObjectType::GAMEOBJECT_TYPE_FLOWER,
		GameObjectType::GAMEOBJECT_TYPE_RMUSHROOM
	};
}

BonusItem::~BonusItem() {}

GameObject::GameObjectType BonusItem::GetCurrentItem() const {
	return _currentItem;
}

RECTF BonusItem::GetBoundingBox(int index) const {
	return _health <= 0 ? RECTF() : GameObject::GetBoundingBox();
}

void BonusItem::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_bonusTexture == nullptr) {
		_bonusTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void BonusItem::HandleStates() {
	_state = static_cast<_State>(_health);

	if (_health == 0 && !IsRemoved()) {
		StartRemoveTimer();
	}
}

void BonusItem::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void BonusItem::Update(DWORD deltaTime, std::vector<Entity*>* collidableEntities, std::vector<Entity*>* collidableTiles, Grid* grid) {
	HandleStates();
	switch (_state) {
		case _State::PICKEDUP:
			_velocity.y -= 0.006f;
			Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);
			break;
		case _State::ROTATE:
			std::random_device randomDevice;
			std::mt19937 rng(randomDevice());
			std::uniform_int_distribution<std::mt19937::result_type> dist(0, 2);

			if (GetTickCount64() % 100 == 0) {
				_currentItem = _possibleItems.at(dist(rng));
			}
			break;
	}
}

void BonusItem::Render() {
	if (!_isActive) {
		return;
	}

	switch (_state) {
		case _State::PICKEDUP:
			{
				std::string animationName = "Shroom";
				switch (_currentItem) {
					case GameObjectType::GAMEOBJECT_TYPE_RMUSHROOM:
						animationName = "Shroom";
						break;
					case GameObjectType::GAMEOBJECT_TYPE_FLOWER:
						animationName = "Flower";
						break;
					case GameObjectType::GAMEOBJECT_TYPE_STAR:
						animationName = "Star";
						break;
				}

				if (GetTickCount64() - _removeStart > _removeTime * 0.75f) {
					_animatedSprite.PlaySpriteAnimation("Spark", _position);
				}
				else {
					_animatedSprite.PlaySpriteAnimation(animationName, _position);
				}
			}
			break;
		case _State::ROTATE:
			_animatedSprite.PlaySpriteAnimation("BonusItem", _position);
			break;
	}
}

void BonusItem::Release() {
	_animatedSprite.Release();
	_bonusTexture = nullptr;
}
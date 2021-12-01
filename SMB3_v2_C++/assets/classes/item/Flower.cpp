#include "../Entity.h"
#include "Flower.h"

Texture* Flower::_flowerTexture = nullptr;

void Flower::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _flowerTexture);
}

Flower::Flower() {
	_renderPriority = 4;
	_removeTime = 50;

	_gravity = 0.002f;

	_emergeTime = 700;

	isPassThroughable = true;
	StartEmergeTimer();
}

Flower::~Flower() {}

bool Flower::IsEmerging() const {
	return _emergeStart != 0;
}

void Flower::StartEmergeTimer() {
	_emergeStart = static_cast<DWORD>(GetTickCount64());
}

RECTF Flower::GetBoundingBox(int) const {
	return _health <= 0 ? RECTF() : GameObject::GetBoundingBox();
}

void Flower::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_flowerTexture == nullptr) {
		_flowerTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void Flower::HandleStates() {
	if (_health == 0 && !IsRemoved()) {
		StartRemoveTimer();
	}
}

void Flower::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void Flower::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (IsEmerging() && GetTickCount64() - _emergeStart > _emergeTime) {
		_renderPriority = 2;
		_emergeStart = 0;
	}

	if (IsEmerging()) {
		_velocity.y = -0.018f;
	}

	HandleStates();
	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);
}

void Flower::Render() {
	_animatedSprite.PlaySpriteAnimation("Flower", _position, _scale);
}

void Flower::Release() {
	_animatedSprite.Release();
	_flowerTexture = nullptr;
}
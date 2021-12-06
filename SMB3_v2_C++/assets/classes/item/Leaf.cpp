#include "../Entity.h"
#include "Leaf.h"

#include <random>

Texture* Leaf::_leafTexture = nullptr;

void Leaf::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _leafTexture);
}

Leaf::Leaf() {
	_renderPriority = 2;
	_removeTime = 50;

	_runSpeed = 0.02f;
	_jumpSpeed = 0.2f;
	_gravity = 0.001f;

	isPassThroughable = true;

	const float BOUNCE_VALUE = 0.88f;
	_velocity.y = -BOUNCE_VALUE;
}

Leaf::~Leaf() {}

RECTF Leaf::GetBoundingBox(int index) const {
	return _health <= 0 ? RECTF() : GameObject::GetBoundingBox();
}

void Leaf::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_leafTexture == nullptr) {
		_leafTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void Leaf::HandleStates() {
	if (_health == 0 && !IsRemoved()) {
		StartRemoveTimer();
	}
}

void Leaf::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void Leaf::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	HandleStates();
	Entity::Update(deltaTime);

	std::random_device randomDevice;
	std::uniform_real_distribution<float> dist(0.0f, 1.2f);

	_velocity.y = _gravity * deltaTime;
	//Voodoo magic
	_position.x += sin(_distance.x) + dist(randomDevice) * _normal.x;

	if (GetTickCount64() % 1000 == 0) {
		_scale.x = -_scale.x;
		_normal.x = -_normal.x;
	}
}

void Leaf::Render() {
	_animatedSprite.PlaySpriteAnimation("Leaf", _position, _scale);
}

void Leaf::Release() {
	_animatedSprite.Release();
	_leafTexture = nullptr;
}
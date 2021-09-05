#include "../Entity.h"
#include "Goomba.h"
#include "Paragoomba.h"

void Paragoomba::_HandleJumping() {
	if (_jumpCount < 4 && _isOnGround) {
		++_jumpCount;

		if (_jumpCount == 4) {
			StartWalkTimer();
		}
	}

	if (_health == 2 && _isOnGround) {
		if (!IsWalking()) {
			float mod = 1.0f;
			if (_jumpCount == 3) {
				mod = 1.8f;
			}

			_velocity.y = -_jumpSpeed * mod;
			_isOnGround = false;
		}
	}
}

Paragoomba::Paragoomba() {
	_jumpCount = 0;

	_health = 2;

	_runSpeed = 0.03f;
	_jumpSpeed = 0.1f;
	_gravity = 0.001f;

	_walkTime = 500;
}

Paragoomba::~Paragoomba() {}

bool Paragoomba::IsWalking() const {
	return _walkStart != 0;
}

void Paragoomba::StartWalkTimer() {
	_walkStart = static_cast<DWORD>(GetTickCount64());
}

void Paragoomba::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (IsWalking() && GetTickCount64() - _walkStart > _walkTime) {
		_jumpCount = 0;
		_walkStart = 0;
	}

	_HandleJumping();
	Goomba::Update(deltaTime, collidableEntities, collidableTiles, grid);
}
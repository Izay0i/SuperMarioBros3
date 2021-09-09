#include "../Entity.h"
#include "Koopa.h"
#include "Parakoopa.h"

Parakoopa::Parakoopa() {
	_health = 4;
}

Parakoopa::~Parakoopa() {}

void Parakoopa::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (_state == _State::FLY) {
		if (_isOnGround) {
			_velocity.y = -_jumpSpeed;
			_isOnGround = false;
		}
	}

	Koopa::Update(deltaTime, collidableEntities, collidableTiles, grid);
}
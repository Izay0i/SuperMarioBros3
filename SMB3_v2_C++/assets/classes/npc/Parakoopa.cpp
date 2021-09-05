#include "../Entity.h"
#include "Koopa.h"
#include "Parakoopa.h"

Parakoopa::Parakoopa() {
	_health = 4;
}

Parakoopa::~Parakoopa() {}

void Parakoopa::SetPosition(D3DXVECTOR2 position) {
	_position = position;
	_originalPos = position;
}

void Parakoopa::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (_health == 4) {
		if (_isOnGround) {
			_velocity.y = -_jumpSpeed;
			_isOnGround = false;
		}
		
		const float MIN_Y_OFFSET = 112.0f;
		if (_position.y <= _originalPos.y) {
			_velocity.y = 0.05f;
		}
		else if (_position.y >= _originalPos.y + MIN_Y_OFFSET) {
			_velocity.y = -0.05f;
		}
	}

	Koopa::Update(deltaTime, collidableEntities, collidableTiles, grid);
}
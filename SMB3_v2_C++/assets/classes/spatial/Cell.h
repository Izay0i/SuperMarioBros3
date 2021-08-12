#pragma once

#include "../Entity.h"

#include <list>

class Cell {
private:
	std::list<Entity*> _entities;

public:
	void MoveEntity(const Entity*&, D3DXVECTOR2);
	void RemoveEntity();
};
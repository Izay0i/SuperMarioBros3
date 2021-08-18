#pragma once

#include "../Entity.h"

#include <unordered_set>

class Cell {
private:
	D3DXVECTOR2 _cellPosition;
	std::unordered_set<Entity*> _entities;

public:
	const static unsigned int CELL_WIDTH = static_cast<int>(GlobalUtil::SCREEN_WIDTH / 2);
	const static unsigned int CELL_HEIGHT = static_cast<int>(GlobalUtil::SCREEN_HEIGHT / 2);

	Cell(D3DXVECTOR2);
	~Cell();

	std::unordered_set<Entity*> GetEntitiesInCell() const;

	void AddEntity(Entity*);
	void RemoveEntity(Entity*);

	void Release();
};
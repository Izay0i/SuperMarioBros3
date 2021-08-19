#pragma once

#include "../Entity.h"

#include <vector>

struct Cell {
	std::vector<Entity*> entities;
};

class Grid {
private:
	const static unsigned int CELL_WIDTH = static_cast<int>(GlobalUtil::SCREEN_WIDTH / 2);
	const static unsigned int CELL_HEIGHT = static_cast<int>(GlobalUtil::SCREEN_HEIGHT / 2);
	
	unsigned int _xCells, _yCells;

	std::vector<std::vector<Cell>> _cells;

public:
	Grid(unsigned int, unsigned int);
	~Grid();

	Cell* GetCell(D3DXVECTOR2);

	void AddEntity(Entity*);
	void AddEntity(Entity*, Cell*);

	void RemoveEntityFromCell(Entity*);

	void Release();
};
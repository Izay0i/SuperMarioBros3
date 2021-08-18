#pragma once

#include "Cell.h"

#include <vector>

class Grid {
private:
	unsigned int _rows, _columns;

	std::vector<std::vector<Cell*>> _cells;
	std::vector<Cell*> _activeCells;

public:
	Grid(unsigned int, unsigned int);
	~Grid();

	Cell* GetCell(D3DXVECTOR2) const;

	std::vector<Cell*> GetActiveCellsInViewport();
	std::vector<Entity*> GetActiveEntities();

	void InitEntities(Entity*);
	void InitEntities(Entity*, D3DXVECTOR2);

	void Update();
	void UpdateCells();
	void UpdateEntitiesPositionInGrid(Entity*);

	void Release();
};
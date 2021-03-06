#pragma once

#include <vector>

#include "../GameObject.h"
#include "../Camera.h"
#include "Cell.h"

class Grid {
private:
	RECTF viewPort;
	int xCells, yCells;
	int rows, columns;

	std::vector<std::vector<Cell*>> cells;
	std::vector<Cell*> activeCells;

public:
	Grid(int, int);
	~Grid();

	Cell* GetCell(int, int);
	Cell* GetCell(float, float);

	void InitObjects(GameObject*);
	void InitObjects(GameObject*, int, int);

	void Update();
	void UpdateGridObjects();
	void UpdateObjectGridPosition(GameObject*);

	std::vector<Cell*> GetActiveCells();
	std::vector<GameObject*> GetActiveObjects();
};

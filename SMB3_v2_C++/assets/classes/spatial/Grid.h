#pragma once

#include "Cell.h"

#include <vector>

class Grid {
private:
	unsigned int _cellWidth, _cellHeight;
	unsigned int _cellSize;

	std::vector<Cell*> _cells;

public:
	void UpdateCells();
};
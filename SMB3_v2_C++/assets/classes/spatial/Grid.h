#pragma once

#include "../Entity.h"

#include <vector>

struct Cell {
	const static unsigned int CELL_WIDTH = static_cast<int>(GlobalUtil::SCREEN_WIDTH / 2);
	const static unsigned int CELL_HEIGHT = static_cast<int>(GlobalUtil::SCREEN_HEIGHT / 2);

	unsigned int indexX, indexY;
	std::vector<Entity*> entities;
};

class Grid {
private:
	friend class Entity;
	
	enum class _GridFileSection {
		GRIDFILE_SECTION_UNKNOWN,
		GRIDFILE_SECTION_GRIDCELLS,
		GRIDFILE_SECTION_POSITIONS
	};

	const unsigned int _MAX_ENTITIES_PER_CELL = 20;

	unsigned int _xCells, _yCells;

	std::vector<std::vector<Cell>> _cells;

	void _ParseGridCells(std::string);
	void _ParsePositions(std::string, std::vector<Entity*>&);

public:
	Grid();
	~Grid();

	Cell* GetCell(unsigned int, unsigned int);
	Cell* GetCell(D3DXVECTOR2);

	void AddEntity(Entity*);
	void AddEntity(Entity*, Cell*);

	void RemoveEntityFromCell(Entity*);

	void ParseData(std::string, std::vector<Entity*>&);

	void Release();
};
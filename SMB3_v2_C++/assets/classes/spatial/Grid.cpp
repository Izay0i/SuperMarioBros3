#include "Grid.h"

void Grid::_ParseGridCells(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 2) {
		return;
	}

	_xCells = std::stoul(tokens.at(0));
	_yCells = std::stoul(tokens.at(1));

	_cells.resize(_xCells, std::vector<Cell>(_yCells));
	for (unsigned int x = 0; x < _xCells; ++x) {
		for (unsigned int y = 0; y < _yCells; ++y) {
			_cells.at(x).at(y).indexX = x;
			_cells.at(x).at(y).indexY = y;
			_cells.at(x).at(y).entities.reserve(_MAX_ENTITIES_PER_CELL);
		}
	}
}

void Grid::_ParsePositions(std::string line, std::vector<Entity*>& entities) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 3) {
		return;
	}

	unsigned int cellX = std::stoul(tokens.at(0));
	unsigned int cellY = std::stoul(tokens.at(1));

	GameObject::GameObjectType objectType = static_cast<GameObject::GameObjectType>(std::stoul(tokens.at(2)));

	for (auto& entity : entities) {
		if (entity->ownerCell == nullptr && entity->GetObjectType() == objectType) {
			Cell* newCell = GetCell(cellX, cellY);
			AddEntity(entity, newCell);
			return;
		}
	}
}

Grid::Grid() {}

Grid::~Grid() {}

Cell* Grid::GetCell(unsigned int x, unsigned int y) {
	if (x < 0) {
		x = 0;
	}
	else if (x >= _xCells) {
		x = _xCells - 1;
	}

	if (y < 0) {
		y = 0;
	}
	else if (y >= _yCells) {
		y = _yCells - 1;
	}

	return &_cells.at(x).at(y);
}

Cell* Grid::GetCell(D3DXVECTOR2 entityPosition) {
	unsigned int x = static_cast<unsigned int>(entityPosition.x / Cell::CELL_WIDTH);
	unsigned int y = static_cast<unsigned int>(entityPosition.y / Cell::CELL_HEIGHT);
	return GetCell(x, y);
}

void Grid::AddEntity(Entity* entity) {
	Cell* newCell = GetCell(entity->GetPosition());
	newCell->entities.emplace_back(entity);
	entity->ownerCell = newCell;
}

void Grid::AddEntity(Entity* entity, Cell* newCell) {
	newCell->entities.emplace_back(entity);
	entity->ownerCell = newCell;
}

void Grid::RemoveEntityFromCell(Entity* entity) {
	std::vector<Entity*>& entities = entity->ownerCell->entities;
	entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
	entity->ownerCell = nullptr;
}

void Grid::ParseData(std::string filePath, std::vector<Entity*>& entities) {
	std::ifstream readFile;
	readFile.open(filePath, std::ios::in);

	if (!readFile.is_open()) {
		OutputDebugStringA("[GRID] Failed to read data\n");
		return;
	}
	
	_GridFileSection gridFileSection = _GridFileSection::GRIDFILE_SECTION_UNKNOWN;

	char str[GlobalUtil::MAX_FILE_LINE];
	while (readFile.getline(str, GlobalUtil::MAX_FILE_LINE)) {
		std::string line(str);

		if (line.empty() || line.front() == '#') {
			continue;
		}

		if (line == "[/]") {
			gridFileSection = _GridFileSection::GRIDFILE_SECTION_UNKNOWN;
			continue;
		}

		if (line == "[GRIDCELLS]") {
			gridFileSection = _GridFileSection::GRIDFILE_SECTION_GRIDCELLS;
			continue;
		}

		if (line == "[POSITIONS]") {
			gridFileSection = _GridFileSection::GRIDFILE_SECTION_POSITIONS;
			continue;
		}

		switch (gridFileSection) {
			case _GridFileSection::GRIDFILE_SECTION_GRIDCELLS:
				_ParseGridCells(line);
				break;
			case _GridFileSection::GRIDFILE_SECTION_POSITIONS:
				_ParsePositions(line, entities);
				break;
		}
	}

	readFile.close();
}

void Grid::Release() {
	_cells.clear();
}
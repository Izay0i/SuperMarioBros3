#include "Grid.h"

void Grid::_ParseGridCells(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 2) {
		return;
	}

	_xCells = std::stoul(tokens.at(0));
	_yCells = std::stoul(tokens.at(1));
	
	_cells.resize(_xCells, std::vector<Cell>(_yCells));

	const unsigned int MAX_ENTITIES_PER_CELL = 20;
	for (unsigned int x = 0; x < _xCells; ++x) {
		for (unsigned int y = 0; y < _yCells; ++y) {
			_cells.at(x).at(y).indexX = x;
			_cells.at(x).at(y).indexY = y;
			_cells.at(x).at(y).entities.reserve(MAX_ENTITIES_PER_CELL);
		}
	}
}

void Grid::_ParsePositions(std::string line, std::vector<Entity*>& entities) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 3) {
		return;
	}

	GameObject::GameObjectType objectType = static_cast<GameObject::GameObjectType>(std::stoul(tokens.at(0)));

	unsigned int cellX = std::stoul(tokens.at(1));
	unsigned int cellY = std::stoul(tokens.at(2));

	for (auto& entity : entities) {
		if (entity->ownerCell == nullptr && entity->GetObjectType() == objectType) {
			Cell* newCell = GetCell(cellX, cellY);
			AddEntity(entity, newCell);
			return;
		}
	}
}

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
	entity->cellIndex = newCell->entities.size() - 1;
}

void Grid::AddEntity(Entity* entity, Cell* newCell) {
	newCell->entities.emplace_back(entity);
	entity->ownerCell = newCell;
	entity->cellIndex = newCell->entities.size() - 1;
}

void Grid::RemoveEntity(Entity* entity) {
	//Vector swap & pop vs erase-remove
	//constant time vs linear time
	//O(1) vs O(n)
	std::vector<Entity*>& entities = entity->ownerCell->entities;
	//Vector swap
	entities.at(entity->cellIndex) = entities.back();
	entities.pop_back();

	//Update the index
	if (static_cast<size_t>(entity->cellIndex) < entities.size()) {
		entities.at(entity->cellIndex)->cellIndex = entity->cellIndex;
	}

	entity->cellIndex = -1;
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
	for (unsigned int i = 0; i < _xCells; ++i) {
		for (unsigned int j = 0; j < _yCells; ++j) {
			_cells.at(i).at(j).entities.clear();
		}
	}
	_cells.clear();
}
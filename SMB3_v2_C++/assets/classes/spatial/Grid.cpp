#include "Grid.h"

void Grid::_ParseCellPositions(std::string line, std::vector<Entity*>& entities) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 3) {
		return;
	}

	unsigned int cellX = std::stoul(tokens.at(0));
	unsigned int cellY = std::stoul(tokens.at(1));

	GameObject::GameObjectType objectType = static_cast<GameObject::GameObjectType>(std::stoul(tokens.at(2)));

	for (auto& entity : entities) {
		if (entity->ownerCell != nullptr) {
			return;
		}

		if (entity->GetObjectType() == objectType) {
			Cell* newCell = GetCell(cellX, cellY);
			if (newCell != entity->ownerCell) {
				RemoveEntityFromCell(entity);
				AddEntity(entity, newCell);
			}

			return;
		}
	}
}

Grid::Grid(unsigned int sceneWidth, unsigned int sceneHeight) {
	//Do float division and then ceil the value to get an extra row and column offscreen
	//Then cast it to unsigned int to truncate the decimals
	_xCells = static_cast<unsigned int>(ceil((static_cast<float>(sceneWidth) / Cell::CELL_WIDTH)));
	_yCells = static_cast<unsigned int>(ceil((static_cast<float>(sceneHeight) / Cell::CELL_HEIGHT)));

	_cells.resize(_xCells, std::vector<Cell>(_yCells));
	for (unsigned int x = 0; x < _xCells; ++x) {
		for (unsigned int y = 0; y < _yCells; ++y) {
			_cells.at(x).at(y).indexX = x;
			_cells.at(x).at(y).indexY = y;
			_cells.at(x).at(y).entities.reserve(_MAX_ENTITIES_PER_CELL);
		}
	}
}

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
	//entity->cellVectorIndex = newCell->entities.size() - 1;
}

void Grid::AddEntity(Entity* entity, Cell* newCell) {
	newCell->entities.emplace_back(entity);
	entity->ownerCell = newCell;
	//entity->cellVectorIndex = newCell->entities.size() - 1;
}

void Grid::RemoveEntityFromCell(Entity* entity) {
	std::vector<Entity*>& entities = entity->ownerCell->entities;
	entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());

	/*
	//Vector swap, does not preserve the order of the elements
	entities.at(entity->cellVectorIndex) = entities.back();
	entities.pop_back();

	//Update vector index
	if (entity->cellVectorIndex < entities.size()) {
		entities.at(entity->cellVectorIndex)->cellVectorIndex = entity->cellVectorIndex;
	}
	*/

	entity->ownerCell = nullptr;
	//entity->cellVectorIndex = std::numeric_limits<unsigned int>::max();
}

void Grid::ParseData(std::string filePath, std::vector<Entity*>& entities) {
	std::ifstream readFile;
	readFile.open(filePath, std::ios::in);

	if (!readFile.is_open()) {
		OutputDebugStringA("[GRID] Failed to read data\n");
		return;
	}
	
	char str[GlobalUtil::MAX_FILE_LINE];
	while (readFile.getline(str, GlobalUtil::MAX_FILE_LINE)) {
		std::string line(str);

		if (line.empty() || line.front() == '#') {
			continue;
		}

		_ParseCellPositions(line, entities);
	}

	readFile.close();
}

void Grid::Release() {
	_cells.clear();
}
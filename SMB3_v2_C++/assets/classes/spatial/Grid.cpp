#include "Grid.h"

Grid::Grid(unsigned int sceneWidth, unsigned int sceneHeight) {
	_xCells = sceneWidth / CELL_WIDTH;
	_yCells = sceneHeight / CELL_HEIGHT;

	const unsigned int ENTITIES_TO_RESERVE = 30;
	_cells.resize(_xCells, std::vector<Cell>(_yCells));
	for (unsigned int x = 0; x < _xCells; ++x) {
		for (unsigned int y = 0; y < _yCells; ++y) {
			_cells.at(x).at(y).entities.reserve(ENTITIES_TO_RESERVE);
		}
	}
}

Grid::~Grid() {}

Cell* Grid::GetCell(D3DXVECTOR2 cellPosition) {
	if (cellPosition.x < 0) {
		cellPosition.x = 0;
	}
	else if (cellPosition.x >= _xCells) {
		cellPosition.x = static_cast<float>(_xCells) - 1;
	}

	if (cellPosition.y < 0) {
		cellPosition.y = 0;
	}
	else if (cellPosition.y >= _yCells) {
		cellPosition.y = static_cast<float>(_yCells) - 1;
	}

	return &_cells
		.at(static_cast<unsigned int>(cellPosition.x))
		.at(static_cast<unsigned int>(cellPosition.y));
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

void Grid::RemoveEntityFromCell(Entity* entity) {
	std::vector<Entity*>& entities = entity->ownerCell->entities;

	entities.at(entity->cellIndex) = entities.back();
	entities.pop_back();

	if (entity->cellIndex < entities.size()) {
		entities.at(entity->cellIndex)->cellIndex = entity->cellIndex;
	}

	entity->cellIndex = std::numeric_limits<unsigned int>::max();
	entity->ownerCell = nullptr;
}

void Grid::Release() {
	for (unsigned int x = _xCells; x < _xCells; ++x) {
		for (unsigned int y = _yCells; y < _yCells; ++y) {
			auto& entities = _cells.at(x).at(y).entities;
			for (auto& entity : entities) {
				entities.pop_back();
			}
		}
	}
	_cells.clear();
}
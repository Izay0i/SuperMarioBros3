#include "Grid.h"

Grid::Grid(unsigned int sceneWidth, unsigned int sceneHeight) {
	_rows = sceneWidth / Cell::CELL_WIDTH;
	_columns = sceneHeight / Cell::CELL_HEIGHT;

	for (unsigned int row = 0; row < _rows; ++row) {
		auto cells = std::vector<Cell*>();
		for (unsigned int column = 0; column < _columns; ++column) {
			cells.emplace_back(new Cell(
				{ static_cast<float>(row), static_cast<float>(column) }
			));
		}
		_cells.emplace_back(cells);
	}
}

Grid::~Grid() {}

Cell* Grid::GetCell(D3DXVECTOR2 entityPosition) const {
	if (entityPosition.x < 0) {
		entityPosition.x = 0;
	}
	else if (entityPosition.x >= _columns) {
		entityPosition.x = static_cast<float>(_columns) - 1;
	}

	if (entityPosition.y < 0) {
		entityPosition.y = 0;
	}
	else if (entityPosition.y >= _rows) {
		entityPosition.y = static_cast<float>(_rows) - 1;
	}

	return _cells
		.at(static_cast<unsigned int>(entityPosition.x))
		.at(static_cast<unsigned int>(entityPosition.y));
}

std::vector<Cell*> Grid::GetActiveCellsInViewport() {
	_activeCells.clear();


	return _activeCells;
}

std::vector<Entity*> Grid::GetActiveEntities() {
	std::vector<Entity*> entities;
	for (const auto& activeCell : _activeCells) {
		for (const auto& entity : activeCell->GetEntitiesInCell()) {
			entities.emplace_back(entity);
		}
	}

	return entities;
}

void Grid::InitEntities(Entity* entity) {
	Cell* cell = GetCell(entity->GetPosition());
	cell->AddEntity(entity);
	//entity->ownerCell = cell;
}

void Grid::InitEntities(Entity* entity, D3DXVECTOR2 entityPosition) {
	Cell* cell = GetCell(entityPosition);
	cell->AddEntity(entity);
	//entity->ownerCell = cell;
}

void Grid::Update() {

	GetActiveCellsInViewport();
	UpdateCells();
}

void Grid::UpdateCells() {
	std::vector<Entity*> gridEntities = GetActiveEntities();

	for (const auto& gridEntity : gridEntities) {
		UpdateEntitiesPositionInGrid(gridEntity);
	}
}

void Grid::UpdateEntitiesPositionInGrid(Entity* entity) {
	Cell* newCell = GetCell(entity->GetPosition());
	/*
	if (newCell != entity->ownerCell) {
		entity->ownerCell->RemoveEntity(entity);
		newCell->AddEntity(entity);
		entity->ownerCell = newCell;
	}
	*/
}

void Grid::Release() {
	for (unsigned int row = 0; row < _rows; ++row) {
		for (unsigned int column = 0; column < _columns; ++column) {
			_cells.at(row).at(column)->Release();
			delete _cells.at(row).at(column);
		}
	}
	_cells.clear();
}
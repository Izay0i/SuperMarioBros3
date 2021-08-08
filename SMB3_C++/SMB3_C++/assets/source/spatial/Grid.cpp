#include "../../headers/spatial/Grid.h"

Grid::Grid(int mapWidth, int mapHeight) {
	xCells = mapWidth / Cell::CELL_WIDTH;
	yCells = mapHeight / Cell::CELL_HEIGHT;

	rows = static_cast<int>(ceil(static_cast<float>(mapWidth) / Cell::CELL_WIDTH));
	columns = static_cast<int>(ceil(static_cast<float>(mapHeight) / Cell::CELL_WIDTH));

	for (int y = 0; y < columns; ++y) {
		auto column = std::vector<Cell*>();
		for (int x = 0; x < rows; ++x) {
			column.push_back(new Cell(x, y));
		}
		cells.push_back(column);
	}
}

Grid::~Grid() {
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < columns; ++j) {
			delete cells.at(i).at(j);
		}
	}
	cells.clear();
}

Cell* Grid::GetCell(int x, int y) {
	if (x < 0) {
		x = 0;
	}
	else if (x >= columns) {
		x = columns - 1;
	}

	if (y < 0) {
		y = 0;
	}
	else if (y >= rows) {
		y = rows - 1;
	}

	return cells.at(x).at(y);
}

Cell* Grid::GetCell(float x, float y) {
	int cX = static_cast<int>(x / Cell::CELL_WIDTH);
	int cY = static_cast<int>(y / Cell::CELL_HEIGHT);

	return GetCell(cX, cY);
}

void Grid::InitObjects(GameObject* object) {
	//not tiles
	if (object->GetObjectID() < 201) {
		Cell* cell = GetCell(object->GetPosition().x, object->GetPosition().y);
		cell->AddObject(object);
		object->ownerCell = cell;
	}
}

void Grid::InitObjects(GameObject* object, int x, int y) {
	if (object->GetObjectID() < 201) {
		Cell* cell = GetCell(x, y);
		cell->AddObject(object);
		object->ownerCell = cell;
	}
}

void Grid::Update() {
	viewPort = Camera::GetInstance()->GetViewport();
	GetActiveCells();
	UpdateGridObjects();
}

void Grid::UpdateGridObjects() {
	std::vector<GameObject*> gridObjects = GetActiveObjects();

	for (auto object : gridObjects) {
		UpdateObjectGridPosition(object);
	}
}

void Grid::UpdateObjectGridPosition(GameObject* object) {
	Cell* newCell = GetCell(object->GetPosition().x, object->GetPosition().y);
	if (newCell != object->ownerCell) {
		//yeah this is big brain time
		object->ownerCell->RemoveObject(object);
		newCell->AddObject(object);
		object->ownerCell = newCell;
	}
}

std::vector<Cell*> Grid::GetActiveCells() {
	activeCells.clear();
	int startX = static_cast<int>(viewPort.left / Cell::CELL_WIDTH);
	int startY = static_cast<int>(viewPort.top / Cell::CELL_HEIGHT);
	int endX = static_cast<int>((viewPort.left + Util::SCREEN_WIDTH) / Cell::CELL_WIDTH);
	int endY = static_cast<int>((viewPort.top + Util::SCREEN_HEIGHT) / Cell::CELL_HEIGHT);

	for (int i = startY - 1; i <= endY; ++i) {
		if (i < 0 || i > rows) {
			continue;
		}

		for (int j = startX - 1; j <= endX; ++j) {
			if (j < 0 || j > columns) {
				continue;
			}

			activeCells.push_back(cells.at(i).at(j));
		}
	}

	return activeCells;
}

std::vector<GameObject*> Grid::GetActiveObjects() {
	std::vector<GameObject*> objects;

	for (auto activeCell : activeCells) {
		for (auto gameObject : activeCell->GetCellObjects()) {
			objects.push_back(gameObject);
		}
	}

	return objects;
}
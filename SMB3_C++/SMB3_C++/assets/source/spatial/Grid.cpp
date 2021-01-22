#include "../../headers/spatial/Grid.h"

Grid::Grid(unsigned int mapWidth, unsigned int mapHeight) {
	xCells = mapWidth / Cell::CELL_WIDTH;
	yCells = mapHeight / Cell::CELL_HEIGHT;

	rows = mapWidth / Cell::CELL_WIDTH;
	columns = mapHeight / Cell::CELL_HEIGHT;

	for (unsigned int y = 0; y < rows; ++y) {
		std::vector<Cell*> row;
		for (unsigned int x = 0; x < columns; ++x) {
			row.push_back(new Cell(x, y));
		}
		cells.push_back(row);
	}
}

Grid::~Grid() {

}

Cell* Grid::GetCell(int x, int y) {
	if (x < 0) {
		x = 0;
	}
	else if (static_cast<unsigned int>(x) >= xCells) {
		x = xCells;
	}

	if (y < 0) {
		y = 0;
	}
	else if (static_cast<unsigned int>(y) >= yCells) {
		y = yCells;
	}

	return cells.at(y).at(x);
}

Cell* Grid::GetCell(float x, float y) {
	int cX = static_cast<int>(x) / Cell::CELL_WIDTH;
	int cY = static_cast<int>(y) / Cell::CELL_HEIGHT;

	return GetCell(cX, cY);
}

void Grid::InitObjects(GameObject* object) {
	//not tiles
	if (object->GetObjectID() < 201) {
		Cell* cell = GetCell(object->GetPosition().x, object->GetPosition().y);
		cell->AddObject(object);
		object->SetOwnerCell(cell);
	}
}

void Grid::InitObjects(GameObject* object, int x, int y) {
	if (object->GetObjectID() < 201) {
		Cell* cell = cells.at(static_cast<unsigned int>(object->GetPosition().x)).at(static_cast<unsigned int>(object->GetPosition().y));
		cell->AddObject(object);
		object->SetOwnerCell(cell);
	}
}

void Grid::Update() {
	viewPort = Camera::GetInstance()->GetViewport();
	GetActiveCells();
	UpdateGridObjects();
}

void Grid::UpdateGridObjects() {
	std::vector<GameObject*> gridObjects = GetActiveObjects();

	auto begin = gridObjects.begin();
	while (begin != gridObjects.end()) {
		UpdateObjectGridPosition(*begin);
		++begin;
	}
}

void Grid::UpdateObjectGridPosition(GameObject* object) {
	Cell* newCell = GetCell(object->GetPosition().x, object->GetPosition().y);
	if (newCell != object->GetOwnerCell()) {
		//yeah this is big brain time
		object->GetOwnerCell()->RemoveObject(object);
		newCell->AddObject(object);
		object->SetOwnerCell(newCell);
	}
}

std::vector<Cell*> Grid::GetActiveCells() {
	activeCells.clear();
	unsigned int startX = static_cast<int>(viewPort.left) / Cell::CELL_WIDTH;
	unsigned int startY = static_cast<int>(viewPort.top) / Cell::CELL_HEIGHT;
	unsigned int endX = static_cast<int>((viewPort.left + Util::SCREEN_WIDTH)) / Cell::CELL_WIDTH;
	unsigned int endY = static_cast<int>((viewPort.top + Util::SCREEN_HEIGHT)) / Cell::CELL_HEIGHT;

	for (unsigned int i = startX; i <= endX; ++i) {
		if (i < 0 || i > columns) {
			continue;
		}

		for (unsigned int j = startY; j <= endY; ++j) {
			if (j < 0 || j > rows) {
				continue;
			}

			activeCells.push_back(cells.at(j).at(i));
		}
	}

	return activeCells;
}

std::vector<GameObject*> Grid::GetActiveObjects() {
	std::vector<GameObject*> objects;

	for (unsigned int i = 0; i < activeCells.size(); ++i) {
		auto begin = activeCells.at(i)->GetCellObjects().begin();
		while (begin != activeCells.at(i)->GetCellObjects().end()) {
			if (dynamic_cast<GameObject*>(*begin)->IsActive()) {
				objects.push_back(*begin);
			}

			++begin;
		}
	}

	return objects;
}
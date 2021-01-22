#include "../../headers/spatial/Cell.h"

Cell::Cell(unsigned int cellX, unsigned int cellY) {
	this->cellX = cellX;
	this->cellY = cellY;
	
	rect.left = static_cast<float>(cellX) * CELL_WIDTH;
	rect.top = static_cast<float>(cellY) * CELL_HEIGHT;
	rect.right = CELL_WIDTH;
	rect.bottom = CELL_HEIGHT;
}

Cell::~Cell() {

}

bool Cell::ContainsObject(RECTF rect) {
	return 
		!(
		(this->rect.left + this->rect.right < rect.left) || 
		(this->rect.left > rect.left + rect.right) ||
		(this->rect.top < rect.top - rect.bottom) ||
		(this->rect.top - this->rect.bottom > rect.top)
		);
}

void Cell::AddObject(GameObject* object) {
	if (objects.find(object) == objects.end()) {
		objects.insert(object);
	}
}

void Cell::RemoveObject(GameObject* object) {
	if (objects.find(object) != objects.end()) {
		objects.erase(object);
	}
}
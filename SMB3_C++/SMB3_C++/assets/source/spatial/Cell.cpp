#include "../../headers/spatial/Cell.h"

Cell::Cell(int cellX, int cellY) {
	this->cellX = cellX;
	this->cellY = cellY;
}

Cell::~Cell() {
	for (auto object : objects) {
		delete object;
		object = nullptr;
	}
	objects.clear();
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
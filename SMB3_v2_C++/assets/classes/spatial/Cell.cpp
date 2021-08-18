#include "Cell.h"

Cell::Cell(D3DXVECTOR2 cellPosition) {
	_cellPosition = cellPosition;
}

Cell::~Cell() {}

std::unordered_set<Entity*> Cell::GetEntitiesInCell() const {
	return _entities;
}

void Cell::AddEntity(Entity* entity) {
	_entities.insert(entity);
}

void Cell::RemoveEntity(Entity* entity) {
	if (_entities.find(entity) != _entities.end()) {
		_entities.erase(entity);
	}
}

void Cell::Release() {
	for (auto entity : _entities) {
		entity = nullptr;
	}
	_entities.clear();
}
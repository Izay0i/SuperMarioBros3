#pragma once

#include <unordered_set>

#include "../GameObject.h"

class Cell {
private:
	std::unordered_set<GameObject*> objects;
	unsigned int cellX, cellY;

	RECTF rect;

public:
	const static unsigned int CELL_WIDTH = 132;
	const static unsigned int CELL_HEIGHT = 132;

	Cell(unsigned int, unsigned int);
	~Cell();

	bool ContainsObject(RECTF);

	std::unordered_set<GameObject*> GetCellObjects() const { return objects; }

	void AddObject(GameObject*);
	void RemoveObject(GameObject*);
};
#pragma once

#include <unordered_set>

#include "../GameObject.h"

class Cell {
private:
	std::unordered_set<GameObject*> objects;
	int cellX, cellY;

	RECTF rect;

public:
	const static int CELL_WIDTH = Util::SCREEN_WIDTH / 2;
	const static int CELL_HEIGHT = Util::SCREEN_HEIGHT / 2;

	Cell(int, int);
	~Cell();

	std::unordered_set<GameObject*> GetCellObjects() const { return objects; }

	void AddObject(GameObject*);
	void RemoveObject(GameObject*);
};
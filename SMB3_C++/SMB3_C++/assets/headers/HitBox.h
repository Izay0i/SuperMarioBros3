#pragma once

#include <vector>

#include <Windows.h>

class HitBox {
private:
	std::vector<RECT> hitboxes;

public:
	RECT GetHitBox(int);

	unsigned int GetWidth(int);
	unsigned int GetHeight(int);

	void AddHitBox(RECT);

	void EnableCollistion();
	void DisableCollision();
};
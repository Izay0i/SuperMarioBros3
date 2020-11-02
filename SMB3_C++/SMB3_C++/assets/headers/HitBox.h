#pragma once

#include <vector>

#include <Windows.h>

class HitBox {
private:
	std::vector<RECT> hitboxes;

public:
	RECT GetHitBox(int);

	void AddHitBox(RECT);

	void EnableCollistion();
	void DisableCollision();
};
#pragma once

#include <vector>

#include <Windows.h>

#include "Util.h"

class HitBox {
private:
	std::vector<RECTF> hitboxes;

public:
	RECTF GetBoundingBox(int = 0) const;

	unsigned int GetWidth(int = 0) const;
	unsigned int GetHeight(int = 0) const;

	void AddHitBox(RECTF);
};
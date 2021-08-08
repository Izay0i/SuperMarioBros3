#pragma once

#include <vector>

#include <Windows.h>

#include "Util.h"

class HitBox {
private:
	std::vector<RECTF> hitboxes;

public:
	RECTF GetBoundingBox(int = 0) const;

	float GetWidth(int = 0) const;
	float GetHeight(int = 0) const;

	void AddHitBox(RECTF);
};
#pragma once

#include "GlobalUtil.h"

#include <vector>

class Hitbox {
private:
	std::vector<RECTF> _hitboxes;

public:
	RECTF GetBoundingBox(int = 0) const;

	void SetBoxWidth(float, int = 0);
	float GetBoxWidth(int = 0) const;
	void SetBoxHeight(float, int = 0);
	float GetBoxHeight(int = 0) const;

	void AddHitbox(RECTF);
};
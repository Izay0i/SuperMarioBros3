#include "Hitbox.h"

RECTF Hitbox::GetBoundingBox(int index) const {
	return _hitboxes.at(index);
}

float Hitbox::GetBoxWidth(int index) const {
	return GetBoundingBox(index).right;
}

float Hitbox::GetBoxHeight(int index) const {
	return GetBoundingBox(index).bottom;
}

void Hitbox::AddHitbox(RECTF hitbox) {
	_hitboxes.emplace_back(hitbox);
}
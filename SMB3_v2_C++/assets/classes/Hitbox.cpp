#include "Hitbox.h"

RECTF Hitbox::GetBoundingBox(int index) const {
	return _hitboxes.empty() ? RECTF() : _hitboxes.at(index);
}

float Hitbox::GetBoxOffsetX(int index) const {
	return GetBoundingBox(index).left;
}

float Hitbox::GetBoxOffsetY(int index) const {
	return GetBoundingBox(index).top;
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
#include "Hitbox.h"

RECTF Hitbox::GetBoundingBox(int index) const {
	return _hitboxes.at(index);
}

void Hitbox::SetBoxWidth(float width, int index) {
	_hitboxes.at(index).right = width;
}

float Hitbox::GetBoxWidth(int index) const {
	return GetBoundingBox(index).right;
}

void Hitbox::SetBoxHeight(float height, int index) {
	_hitboxes.at(index).bottom = height;
}

float Hitbox::GetBoxHeight(int index) const {
	return GetBoundingBox(index).bottom;
}

void Hitbox::AddHitbox(RECTF hitbox) {
	_hitboxes.emplace_back(hitbox);
}
#include "../headers/HitBox.h"

RECTF HitBox::GetBoundingBox(int id) const {	
	return hitboxes.at(id);
}

float HitBox::GetWidth(int id) const {
	return hitboxes.at(id).right;
}

float HitBox::GetHeight(int id) const {
	return hitboxes.at(id).bottom;
}

void HitBox::AddHitBox(RECTF hitbox) {
	hitboxes.push_back(hitbox);
}
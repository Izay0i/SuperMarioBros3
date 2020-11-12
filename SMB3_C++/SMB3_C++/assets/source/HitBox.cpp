#include "../headers/HitBox.h"

RECTF HitBox::GetBoundingBox(int id) const {	
	return hitboxes.at(id);
}

unsigned int HitBox::GetWidth(int id) const {
	return hitboxes.at(id).right;
}

unsigned int HitBox::GetHeight(int id) const {
	return hitboxes.at(id).bottom;
}

void HitBox::AddHitBox(RECTF hitbox) {
	hitboxes.push_back(hitbox);
}
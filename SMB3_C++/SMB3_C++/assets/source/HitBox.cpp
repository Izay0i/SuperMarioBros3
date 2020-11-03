#include "../headers/HitBox.h"

RECT HitBox::GetHitBox(int id = 0) {
	return hitboxes.at(id);
}

unsigned int HitBox::GetWidth(int id = 0) {
	return hitboxes.at(id).right;
}

unsigned int HitBox::GetHeight(int id = 0) {
	return hitboxes.at(id).bottom;
}

void HitBox::AddHitBox(RECT hitbox) {
	hitboxes.push_back(hitbox);
}
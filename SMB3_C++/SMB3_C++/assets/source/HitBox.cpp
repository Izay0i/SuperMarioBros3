#include "../headers/HitBox.h"

RECT HitBox::GetHitBox(int id = 0) {
	return hitboxes.at(id);
}

void HitBox::AddHitBox(RECT hitbox) {
	hitboxes.push_back(hitbox);
}
#include "Entity.h"
#include "Tile.h"

void Tile::_ParseSprites(std::string) {}

void Tile::AddHitbox(RECTF hitbox) {
	_hitbox.AddHitbox(hitbox);
}

void Tile::HandleStates() {}

void Tile::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void Tile::Release() {}

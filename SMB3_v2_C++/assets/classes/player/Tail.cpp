#include "Player.h"
#include "Tail.h"

void Tail::_ParseSprites(std::string) {}

Tail::Tail(Player* player) {
	_player = player;
	_hitbox.AddHitbox({ 0, 0, 0, 0 });
}

Tail::~Tail() {}

void Tail::HandleStates() {}

void Tail::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void Tail::Update(DWORD, std::vector<Entity*>*, std::vector<Entity*>*, Grid*) {
	if (_player->IsAttacking()) {
		_hitbox.SetBoxWidth(_TAIL_WIDTH);
		_hitbox.SetBoxHeight(_TAIL_HEIGHT);
	}
	else {
		_hitbox.SetBoxWidth(0);
		_hitbox.SetBoxHeight(0);
	}
	
	_position.x = _player->_position.x - _player->GetBoxWidth();
	_position.y = _player->_position.y + 22.0f;
}

void Tail::Release() {
	_player = nullptr;
}

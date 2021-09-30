#include "Player.h"
#include "Tail.h"

void Tail::_ParseSprites(std::string) {}

Tail::Tail(Player* player) {
	_player = player;
	_objectType = GameObjectType::GAMEOBJECT_TYPE_TAIL;
	_hitbox.AddHitbox({ 0, 0, _TAIL_WIDTH, _TAIL_HEIGHT });
}

Tail::~Tail() {}

void Tail::HandleStates() {}

void Tail::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void Tail::Update(DWORD, std::vector<Entity*>*, std::vector<Entity*>*, Grid*) {
	_position.x = _player->_position.x - _player->GetBoxWidth();
	_position.y = _player->_position.y + 11.0f;
}

void Tail::Release() {
	_player = nullptr;
}

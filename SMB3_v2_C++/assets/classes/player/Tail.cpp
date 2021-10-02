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

void Tail::HandleCollisionResult(
	LPCOLLISIONEVENT result, 
	D3DXVECTOR2& minTime, 
	D3DXVECTOR2& offset, 
	D3DXVECTOR2& normal, 
	D3DXVECTOR2& relativeDistance) 
{
	Entity* eventEntity = result->entity;
	D3DXVECTOR2 eventNormal = result->normal;

	if (eventEntity == nullptr) {
		return;
	}

	switch (eventEntity->GetObjectType()) {
		case GameObjectType::GAMEOBJECT_TYPE_QUESTIONBLOCK:
			OutputDebugStringA("QuestionBlock\n");
			break;
	}
}

void Tail::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);
	
	_position.x = _player->_position.x - _player->GetBoxWidth();
	_position.y = _player->_position.y + 11.0f;
}

void Tail::Release() {
	_player = nullptr;
}

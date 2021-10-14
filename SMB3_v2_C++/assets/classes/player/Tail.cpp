#include "../Entity.h"
#include "Tail.h"

void Tail::_ParseSprites(std::string) {}

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
}

void Tail::Release() {
	
}

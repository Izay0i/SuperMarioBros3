#include "../Entity.h"
#include "Tail.h"
#include "../EntityList.h"

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

	switch (eventEntity->GetObjectType()) {
		case GameObjectType::GAMEOBJECT_TYPE_GOOMBA:
		case GameObjectType::GAMEOBJECT_TYPE_PARAGOOMBA:
			{
				Goomba* goomba = dynamic_cast<Goomba*>(eventEntity);
				goomba->TakeDamage();
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_KOOPA:
		case GameObjectType::GAMEOBJECT_TYPE_PARAKOOPA:
			{
				Koopa* koopa = dynamic_cast<Koopa*>(eventEntity);
				koopa->TakeDamage();
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_PIRAPLANT:
		case GameObjectType::GAMEOBJECT_TYPE_VENUSPLANT:
			{
				PiranaPlant* piranaPlant = dynamic_cast<PiranaPlant*>(eventEntity);
				piranaPlant->TakeDamage();
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_BOOMERBRO:
			{
				BoomerBro* boomerBro = dynamic_cast<BoomerBro*>(eventEntity);
				boomerBro->TakeDamage();
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_COIN:
			{
				Coin* coin = dynamic_cast<Coin*>(eventEntity);
				//Is brick
				if (coin->GetHealth() == 3) {
					coin->SetHealth(-1);
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_QUESTIONBLOCK:
			{
				QuestionBlock* questionBlock = dynamic_cast<QuestionBlock*>(eventEntity);
				questionBlock->TakeDamage();
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_SHINYBRICK:
			{
				ShinyBrick* shinyBrick = dynamic_cast<ShinyBrick*>(eventEntity);
				if (shinyBrick->GetExtraData().size() != 3) {
					shinyBrick->TakeDamage();
				}
				//Is not coin
				else if (shinyBrick->GetHealth() != 3 && shinyBrick->GetExtraData().size() == 3) {
					shinyBrick->SetHealth(-1);
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_PBLOCK:
			{
				PBlock* pBlock = dynamic_cast<PBlock*>(eventEntity);
				pBlock->TakeDamage();
			}
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

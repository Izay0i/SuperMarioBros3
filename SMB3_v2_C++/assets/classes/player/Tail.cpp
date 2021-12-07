#include "../Entity.h"
#include "Tail.h"
#include "../EntityList.h"
#include "../audio/AudioService.h"

Texture* Tail::_tailTexture = nullptr;

void Tail::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _tailTexture);
}

Tail::Tail() {
	_renderPriority = 0;
	_bounceSpeed = 0.23f;

	_touchedEntity = nullptr;

	isPassThroughable = true;
}

Tail::~Tail() {}

void Tail::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_tailTexture == nullptr) {
		_tailTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void Tail::HandleStates() {}

void Tail::HandleCollisionResult(
	LPCOLLISIONEVENT result, 
	D3DXVECTOR2& minTime, 
	D3DXVECTOR2& offset, 
	D3DXVECTOR2& normal, 
	D3DXVECTOR2& relativeDistance) {}

void Tail::HandleOverlap(Entity* entity) {
	switch (entity->GetObjectType()) {
		case GameObjectType::GAMEOBJECT_TYPE_GOOMBA:
		case GameObjectType::GAMEOBJECT_TYPE_PARAGOOMBA:
		case GameObjectType::GAMEOBJECT_TYPE_KOOPA:
		case GameObjectType::GAMEOBJECT_TYPE_PARAKOOPA:
		case GameObjectType::GAMEOBJECT_TYPE_PIRANHAPLANT:
		case GameObjectType::GAMEOBJECT_TYPE_VENUSPLANT:
		case GameObjectType::GAMEOBJECT_TYPE_BOOMERANGBRO:
		case GameObjectType::GAMEOBJECT_TYPE_DRYBONES:
		case GameObjectType::GAMEOBJECT_TYPE_FORTRESSBOSS:
			_touchedEntity = entity;
			break;
	}

	switch (entity->GetObjectType()) {
		case GameObjectType::GAMEOBJECT_TYPE_GOOMBA:
		case GameObjectType::GAMEOBJECT_TYPE_PARAGOOMBA:
			{
				Goomba* goomba = dynamic_cast<Goomba*>(entity);
				goomba->animationName = "Walk";
				goomba->SetHealth(0);
				goomba->SetScale({ 1.0f, -1.0f });
				goomba->SetVelocity({ 0.0f, -_bounceSpeed });
			}

			AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_KICK);
			break;
		case GameObjectType::GAMEOBJECT_TYPE_KOOPA:
		case GameObjectType::GAMEOBJECT_TYPE_PARAKOOPA:
			{
				Koopa* koopa = dynamic_cast<Koopa*>(entity);
				koopa->SetHealth(2);
				koopa->StartRetractTimer();
				koopa->SetScale({ 1.0f, -1.0f });
				koopa->SetVelocity({ 0.0f, -_bounceSpeed });
			}

			AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_KICK);
			break;
		case GameObjectType::GAMEOBJECT_TYPE_PIRANHAPLANT:
		case GameObjectType::GAMEOBJECT_TYPE_VENUSPLANT:
			{
				PiranaPlant* piranaPlant = dynamic_cast<PiranaPlant*>(entity);
				piranaPlant->TakeDamage();
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_BOOMERANGBRO:
			{
				BoomerBro* boomerBro = dynamic_cast<BoomerBro*>(entity);
				boomerBro->TakeDamage();
			}

			AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_KICK);
			break;
		case GameObjectType::GAMEOBJECT_TYPE_DRYBONES:
			{
				DryBones* dryBones = dynamic_cast<DryBones*>(entity);
				dryBones->TakeDamage();
			}

			AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_BLOCKBREAK);
			break;
		case GameObjectType::GAMEOBJECT_TYPE_FORTRESSBOSS:
			{
				FortressBoss* fortressBoss = dynamic_cast<FortressBoss*>(entity);
				fortressBoss->TakeDamage();
			}

			AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_KICK);
			break;
		case GameObjectType::GAMEOBJECT_TYPE_COIN:
			{
				Coin* coin = dynamic_cast<Coin*>(entity);
				//Is brick
				if (coin->GetHealth() == 3) {
					coin->SetHealth(-1);
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_QUESTIONBLOCK:
			{
				QuestionBlock* questionBlock = dynamic_cast<QuestionBlock*>(entity);
				questionBlock->TakeDamage();
			}

			AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_BUMP);
			break;
		case GameObjectType::GAMEOBJECT_TYPE_SHINYBRICK:
			{
				ShinyBrick* shinyBrick = dynamic_cast<ShinyBrick*>(entity);
				//Has items
				if (shinyBrick->GetExtraData().size() != 3) {
					shinyBrick->TakeDamage();

					AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_BUMP);
				}
				//Is empty
				else if (shinyBrick->GetHealth() != 3 && shinyBrick->GetExtraData().size() == 3) {
					shinyBrick->SetHealth(-1);

					AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_BLOCKBREAK);
				}
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_PBLOCK:
			{
				PBlock* pBlock = dynamic_cast<PBlock*>(entity);
				pBlock->TakeDamage();
			}

			AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_THWOMP);
			break;
	}
}

void Tail::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (_touchedEntity != nullptr && GetTickCount64() % 100 == 0) {
		_touchedEntity = nullptr;
	}

	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);
}

void Tail::Render() {
	//_animatedSprite.PlaySpriteAnimation("Tail", _position);

	if (_touchedEntity != nullptr) {
		_animatedSprite.PlaySpriteAnimation("Spark", _touchedEntity->GetPosition());
	}
}

void Tail::Release() {
	_animatedSprite.Release();
	_tailTexture = nullptr;
}
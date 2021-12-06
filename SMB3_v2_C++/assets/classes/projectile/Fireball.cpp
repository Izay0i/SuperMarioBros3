#include "../Entity.h"
#include "Fireball.h"
#include "../EntityList.h"
#include "../audio/AudioService.h"

Texture* Fireball::_fireballTexture = nullptr;

void Fireball::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _fireballTexture);
}

Fireball::Fireball() {
	_renderPriority = 2;
	_removeTime = 300;

	_travelSpeed = 0.0012f;
	_bounceSpeed = 0.2f;
	_gravity = 0.002f;

	_aliveTime = 8000;

	isPassThroughable = true;
	StartAliveTimer();
}

Fireball::~Fireball() {}

bool Fireball::IsAlive() const {
	return _aliveStart != 0;
}

void Fireball::StartAliveTimer() {
	_aliveStart = static_cast<DWORD>(GetTickCount64());
}

RECTF Fireball::GetBoundingBox(int index) const {
	return _health <= 0 ? RECTF() : GameObject::GetBoundingBox();
}

void Fireball::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_fireballTexture == nullptr) {
		_fireballTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);

	_runSpeed = _objectType == GameObjectType::GAMEOBJECT_TYPE_PLAYERFIREBALL ? 0.2f : 0.05f;
}

void Fireball::HandleStates() {
	_state = static_cast<_State>(_health);

	switch (_state) {
		case _State::BOUNCE:
			_velocity.x = _runSpeed * _normal.x;
			break;
		case _State::EXPLODE:
			_velocity = { 0.0f, 0.0f };

			if (_health == 0 && !IsRemoved()) {
				StartRemoveTimer();
			}
			break;
	}
}

void Fireball::HandleCollisionResult(
	LPCOLLISIONEVENT result, 
	D3DXVECTOR2& minTime, 
	D3DXVECTOR2& offset, 
	D3DXVECTOR2& normal, 
	D3DXVECTOR2& relativeDistance) 
{
	Entity* eventEntity = result->entity;
	D3DXVECTOR2 eventNormal = result->normal;

	switch (_objectType) {
		case GameObjectType::GAMEOBJECT_TYPE_PLAYERFIREBALL:
			if (eventNormal.y == -1.0f) {
				_velocity.y = -_bounceSpeed;
			}

			switch (eventEntity->GetObjectType()) {
				case GameObjectType::GAMEOBJECT_TYPE_GOOMBA:
				case GameObjectType::GAMEOBJECT_TYPE_PARAGOOMBA:
				case GameObjectType::GAMEOBJECT_TYPE_KOOPA:
				case GameObjectType::GAMEOBJECT_TYPE_PARAKOOPA:
					eventEntity->SetHealth(0);
					eventEntity->SetScale({ 1.0f, -1.0f });
					eventEntity->SetVelocity({ 0.0f, -_bounceSpeed });

					AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_KICK);
					break;
				case GameObjectType::GAMEOBJECT_TYPE_VENUSFIREBALL:
				case GameObjectType::GAMEOBJECT_TYPE_DRYBONES:
					TakeDamage();
					break;
				case GameObjectType::GAMEOBJECT_TYPE_COIN:
					{
						Coin* coin = dynamic_cast<Coin*>(eventEntity);
						if (coin->GetHealth() == 3 && eventNormal.x != 0.0f) {
							coin->SetHealth(-1);
							TakeDamage();
						}
					}
					break;
				case GameObjectType::GAMEOBJECT_TYPE_QUESTIONBLOCK:
					{
						QuestionBlock* questionBlock = dynamic_cast<QuestionBlock*>(eventEntity);
						if (eventNormal.x != 0.0f) {
							questionBlock->TakeDamage();
							TakeDamage();

							AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_BUMP);
						}
					}
					break;
				case GameObjectType::GAMEOBJECT_TYPE_SHINYBRICK:
					{
						ShinyBrick* shinyBrick = dynamic_cast<ShinyBrick*>(eventEntity);
						if (eventNormal.x != 0.0f) {
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
							TakeDamage();
						}
					}
					break;
				case GameObjectType::GAMEOBJECT_TYPE_PBLOCK:
					{
						PBlock* pBlock = dynamic_cast<PBlock*>(eventEntity);
						if (eventNormal.x != 0.0f) {
							pBlock->TakeDamage();
							TakeDamage();

							AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_THWOMP);
						}
					}
					break;
				case GameObjectType::GAMEOBJECT_TYPE_TRIGGER:
				case GameObjectType::GAMEOBJECT_TYPE_MOVINGCEILING:
				case GameObjectType::GAMEOBJECT_TYPE_TILE:
					if (eventNormal.x != 0.0f) {
						TakeDamage();
					}
					break;
				//Ignore these entities
				case GameObjectType::GAMEOBJECT_TYPE_MARIO:
				case GameObjectType::GAMEOBJECT_TYPE_LUIGI:
				case GameObjectType::GAMEOBJECT_TYPE_PODOBOO:
				case GameObjectType::GAMEOBJECT_TYPE_ROTODISC:
				case GameObjectType::GAMEOBJECT_TYPE_REDMUSHROOM:
				case GameObjectType::GAMEOBJECT_TYPE_GREENMUSHROOM:
				case GameObjectType::GAMEOBJECT_TYPE_FLOWER:
				case GameObjectType::GAMEOBJECT_TYPE_STAR:
				case GameObjectType::GAMEOBJECT_TYPE_BONUSITEM:
				case GameObjectType::GAMEOBJECT_TYPE_ORB:
				case GameObjectType::GAMEOBJECT_TYPE_DOOR:
				case GameObjectType::GAMEOBJECT_TYPE_ONEWAYPLATFORM:
					//Pokemon Black & White, Black 2 & White 2 stories were the peak of the franchise, change my mind
					break;
				default:
					eventEntity->TakeDamage();
					TakeDamage();
			}
			break;
		case GameObjectType::GAMEOBJECT_TYPE_VENUSFIREBALL:
			minTime = { 1.0f, 1.0f };
			offset = normal = relativeDistance = { 0.0f, 0.0f };
			
			switch (eventEntity->GetObjectType()) {
				case GameObjectType::GAMEOBJECT_TYPE_PLAYERFIREBALL:
					TakeDamage();
					break;
			}
			break;
	}
}

void Fireball::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (IsAlive() && GetTickCount64() - _aliveStart > _aliveTime) {
		_health = -1;
		_aliveStart = 0;
	}

	HandleStates();
	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);
	
	if (_objectType == GameObjectType::GAMEOBJECT_TYPE_VENUSFIREBALL) {
		_velocity.y = -_travelSpeed * _normal.y * deltaTime;
	}
}

void Fireball::Render() {
	switch (_state) {
		case _State::BOUNCE:
			_animatedSprite.PlaySpriteAnimation("Fireball", _position, { _normal.x, 1.0f });
			break;
		case _State::EXPLODE:
			_animatedSprite.PlaySpriteAnimation("Explode", _position);
			break;
	}
}

void Fireball::Release() {
	_animatedSprite.Release();
	_fireballTexture = nullptr;
}
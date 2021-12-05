#include "../Entity.h"
#include "../item/Coin.h"
#include "ShinyBrick.h"
#include "PBlock.h"

Texture* PBlock::_pBlockTexture = nullptr;

void PBlock::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _pBlockTexture);
}

PBlock::PBlock() {
	_renderPriority = 4;
	_health = 2;
	_activateTime = 8000;
}

PBlock::~PBlock() {}

bool PBlock::IsActivated() const {
	return _activateStart != 0;
}

void PBlock::StartActivationTimer() {
	_activateStart = static_cast<DWORD>(GetTickCount64());
}

RECTF PBlock::GetBoundingBox(int) const {
	return _health <= 1 ? RECTF() : GameObject::GetBoundingBox();
}

void PBlock::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_pBlockTexture == nullptr) {
		_pBlockTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void PBlock::TakeDamage() {
	if (_health > 1) {
		--_health;
		StartActivationTimer();

		tookDamage = true;
	}
}

void PBlock::HandleStates() {
	_state = static_cast<_State>(_health);
}

void PBlock::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void PBlock::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	HandleStates();

	if (IsActivated() && GetTickCount64() - _activateStart > _activateTime) {
		_activateStart = 0;

		hasEnded = true;
	}
	
	for (unsigned int i = 0; i < collidableEntities->size(); ++i) {
		Entity* entity = collidableEntities->at(i);
		if (entity->IsActive() && entity->GetHealth() > 0) {
			switch (entity->GetObjectType()) {
				case GameObjectType::GAMEOBJECT_TYPE_COIN:
				{
					Coin* coin = dynamic_cast<Coin*>(entity);
					coin->SetHealth(IsActivated() ? 3 : 1);
				}
				break;
			case GameObjectType::GAMEOBJECT_TYPE_SHINYBRICK:
				{
					ShinyBrick* shinyBrick = dynamic_cast<ShinyBrick*>(entity);
					if (shinyBrick->GetExtraData().size() == 3) {
						shinyBrick->SetHealth(IsActivated() ? 3 : 2);
					}
				}
				break;
			}
		}
	}
}

void PBlock::Render() {
	switch (_state) {
		case _State::ACTIVE:
			_animatedSprite.PlaySpriteAnimation("PBlock", _position);
			break;
		case _State::INACTIVE:
			_animatedSprite.PlaySpriteAnimation("Blank", _position);
			break;
	}
}

void PBlock::Release() {
	_animatedSprite.Release();
	_pBlockTexture = nullptr;
}
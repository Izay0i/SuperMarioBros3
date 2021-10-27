#include "../SceneManager.h"
#include "../Entity.h"
#include "../item/Mushroom.h"
#include "../item/Leaf.h"
#include "../item/Coin.h"
#include "QuestionBlock.h"

Texture* QuestionBlock::_questionTexture = nullptr;

void QuestionBlock::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _questionTexture);
}

QuestionBlock::QuestionBlock() {
	_renderPriority = 3;
	_health = 2;
	_jumpSpeed = 0.2f;
	_gravity = 0.001f;
}

QuestionBlock::~QuestionBlock() {}

RECTF QuestionBlock::GetBoundingBox(int) const {
	return GameObject::GetBoundingBox();
}

void QuestionBlock::SetPosition(D3DXVECTOR2 position) {
	_position = position;
	_originalPos = position;
}

Entity* QuestionBlock::SpawnItem(int currentHealth) {
	Entity* item = nullptr;
	//extra data size:
	//3 - Coin
	//6 - HP: 1 -> RMushroom
	//    HP: 2, 3, 4 -> Leaf

	switch (_extraData.size()) {
		case 3:
			//Coin
			item = SceneManager::GetInstance()->GetCurrentScene()->CreateEntityFromData(
				_extraData.at(0),
				_extraData.at(1),
				_extraData.at(2)
			);
			item->SetHealth(2);
			item->SetVelocity({ 0.0f, -0.20f });
			item->SetPosition({ _originalPos.x, _originalPos.y - item->GetBoxHeight() });
			break;
		case 6:
			if (currentHealth >= 2) {
				//Leaf
				item = SceneManager::GetInstance()->GetCurrentScene()->CreateEntityFromData(
					_extraData.at(3),
					_extraData.at(4),
					_extraData.at(5)
				);
				item->SetVelocity({ 0.0f, -0.38f });
				item->SetPosition({ _originalPos.x, _originalPos.y - item->GetBoxHeight() });
			}
			else {
				//Mushroom
				item = SceneManager::GetInstance()->GetCurrentScene()->CreateEntityFromData(
					_extraData.at(0),
					_extraData.at(1),
					_extraData.at(2)
				);
				item->SetPosition({ _originalPos.x, _originalPos.y - item->GetBoxHeight() / 3.0f });
				dynamic_cast<Mushroom*>(item)->StartEmergeTimer();
			}
			break;
	}

	tookDamage = false;
	_extraData.clear();

	return item;
}

void QuestionBlock::ParseData(
	std::string dataPath,
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_questionTexture == nullptr) {
		_questionTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void QuestionBlock::TakeDamage() {
	if (_health > 1) {
		--_health;

		if (_position.y >= _originalPos.y - _MAX_Y_OFFSET) {
			_velocity.y = -_jumpSpeed;
		}

		tookDamage = true;
	}
}

void QuestionBlock::HandleStates() {
	_state = static_cast<_State>(_health);
}

void QuestionBlock::HandleCollisionResult(
	LPCOLLISIONEVENT result, 
	D3DXVECTOR2& minTime, 
	D3DXVECTOR2& offset, 
	D3DXVECTOR2& normal, 
	D3DXVECTOR2& relativeDistance) {}

void QuestionBlock::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	HandleStates();
	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);
	//GameObject::Update(deltaTime);

	if (_position.y < _originalPos.y) {
		_velocity.y += _gravity * deltaTime;
	}
	else {
		_velocity.y = 0.0f;

		if (_position.y >= _originalPos.y) {
			_position = _originalPos;
		}
		else if (_position.y < _originalPos.y - _MAX_Y_OFFSET) {
			_position.y = _originalPos.y - _MAX_Y_OFFSET;
		}
	}
	_position += _distance;
}

void QuestionBlock::Render() {
	switch (_state) {
		case _State::PUSHED:
			_animatedSprite.PlaySpriteAnimation("Blank", _position);
			break;
		case _State::ROTATE:
			_animatedSprite.PlaySpriteAnimation("QBlock", _position);
			break;
	}
}

void QuestionBlock::Release() {
	_animatedSprite.Release();
	_questionTexture = nullptr;
}
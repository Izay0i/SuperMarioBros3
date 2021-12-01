#include "../SceneManager.h"
#include "../Entity.h"
#include "../item/Mushroom.h"
#include "../item/Coin.h"
#include "PBlock.h"
#include "../effect/BrickDebris.h"
#include "ShinyBrick.h"
#include "../audio/AudioService.h"

Texture* ShinyBrick::_brickTexture = nullptr;

void ShinyBrick::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _brickTexture);
}

ShinyBrick::ShinyBrick() {
	_renderPriority = 3;
	_health = 2;
	_jumpSpeed = 0.2f;
	_gravity = 0.001f;
	_removeTime = 1;
}

ShinyBrick::~ShinyBrick() {}

RECTF ShinyBrick::GetBoundingBox(int) const {
	return _health <= 0 ? RECTF() : GameObject::GetBoundingBox();
}

void ShinyBrick::SetPosition(D3DXVECTOR2 position) {
	_position = position;
	_originalPos = position;
}

Entity* ShinyBrick::SpawnItem() {
	Entity* item = SceneManager::GetInstance()->GetCurrentScene()->CreateEntityFromData(
		_extraData.at(3),
		_extraData.at(4),
		_extraData.at(5)
	);

	switch (item->GetObjectType()) {
		case GameObjectType::GAMEOBJECT_TYPE_GREENMUSHROOM:
			item->SetPosition({ _originalPos.x, _originalPos.y - item->GetBoxHeight() / 3.0f });
			dynamic_cast<Mushroom*>(item)->StartEmergeTimer();

			AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_MUSHROOMAPPEARS);
			break;
		case GameObjectType::GAMEOBJECT_TYPE_COIN:
			item->SetHealth(2);
			item->SetPosition({ _originalPos.x, _originalPos.y - item->GetBoxHeight() });
			dynamic_cast<Coin*>(item)->StartPopUpTimer();

			AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_COIN);
			break;
		case GameObjectType::GAMEOBJECT_TYPE_PBLOCK:
			item->SetPosition({ _originalPos.x, _originalPos.y - item->GetBoxHeight() });
			break;
	}

	--_itemCount;
	if (_itemCount == 0) {
		_extraData.clear();
	}

	return item;
}

BrickDebris* ShinyBrick::SpawnDebris() {
	BrickDebris* brickDebris = dynamic_cast<BrickDebris*>(
		SceneManager::GetInstance()->GetCurrentScene()->CreateEntityFromData(
			_extraData.at(0), 
			_extraData.at(1), 
			_extraData.at(2)
		)
	);
	brickDebris->SetPosition(_position);
	brickDebris->StartRemoveTimer();
	return brickDebris;
}

void ShinyBrick::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_brickTexture == nullptr) {
		_brickTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
	_itemCount = _extraData.size() > 3 ? std::stoul(_extraData.back()) : 0;
}

void ShinyBrick::TakeDamage() {
	if (_health > 1 && _itemCount == 1) {
		--_health;
	}

	if (_itemCount > 0) {
		tookDamage = true;
	}

	if (_state != _State::PUSHED) {
		if (_position.y >= _originalPos.y - _MAX_Y_OFFSET) {
			_velocity.y = -_jumpSpeed;
		}
	}
}

void ShinyBrick::HandleStates() {
	_state = static_cast<_State>(_health);
	isPassThroughable = _state == _State::COIN;

	switch (_state) {
		case _State::COIN:
			_gravity = 0.0f;
			break;
		default:
			_gravity = 0.001f;
	}
}

void ShinyBrick::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void ShinyBrick::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	HandleStates();
	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);

	if (_state != _State::COIN) {
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
}

void ShinyBrick::Render() {
	switch (_state) {
		case _State::PUSHED:
			_animatedSprite.PlaySpriteAnimation("Blank", _position);
			break;
		case _State::ROTATE:
			_animatedSprite.PlaySpriteAnimation("Brick", _position);
			break;
		case _State::COIN:
			_animatedSprite.PlaySpriteAnimation("Coin", _position);
			break;
	}
}

void ShinyBrick::Release() {
	_animatedSprite.Release();
	_brickTexture = nullptr;
}
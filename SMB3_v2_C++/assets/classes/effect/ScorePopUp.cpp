#include "../player/Player.h"
#include "ScorePopUp.h"

Texture* ScorePopUp::_scoreTexture = nullptr;

void ScorePopUp::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _scoreTexture);
}

ScorePopUp::ScorePopUp(Player* player) {
	_player = player;
	
	_gravity = -0.001f;

	_index = 2;
	_score = 100;
	_scores = { 10, 50, 100, 200, 400, 800, 1000, 2000, 4000, 8000, 10000 };
	_floatTime = 1000;
}

ScorePopUp::~ScorePopUp() {}

void ScorePopUp::GetEntity(Entity* entity) {
	switch (entity->GetObjectType()) {
		//1000 points each
		case GameObjectType::GAMEOBJECT_TYPE_REDMUSHROOM:
		case GameObjectType::GAMEOBJECT_TYPE_LEAF:
		case GameObjectType::GAMEOBJECT_TYPE_FLOWER:
			_score = _scores.at(6);
			_player->_score += _score;
			break;
		//1-UP
		case GameObjectType::GAMEOBJECT_TYPE_GREENMUSHROOM:
			_score = _scores.back();
			_player->_lives += 1;
			break;
		//50 points each
		case GameObjectType::GAMEOBJECT_TYPE_COIN:
			_score = entity->GetHealth() == -1 ? _scores.at(2) : _scores.at(1);
			_player->_score += _score;
			_player->_coins += 1;
			break;
		//10 for empty, 50 for non-empty
		case GameObjectType::GAMEOBJECT_TYPE_SHINYBRICK:
			if (entity->GetExtraData().size() == 3) {
				//Is coin
				if (entity->GetHealth() == -2) {
					_player->_coins += 1;
				}
				//Is empty;
				else {
					_score = _scores.front();
				}
			}
			//Is not empty
			else if (entity->GetExtraData().size() != 3) {
				_score = _scores.at(1);
			}
			_player->_score += _score;
			break;
		//100+ each time the player hits an npc and is in the air
		//Caps at 10000 == 1-UP
		case GameObjectType::GAMEOBJECT_TYPE_GOOMBA:
		case GameObjectType::GAMEOBJECT_TYPE_PARAGOOMBA:
		case GameObjectType::GAMEOBJECT_TYPE_KOOPA:
		case GameObjectType::GAMEOBJECT_TYPE_PARAKOOPA:
		case GameObjectType::GAMEOBJECT_TYPE_PIRANHAPLANT:
		case GameObjectType::GAMEOBJECT_TYPE_VENUSPLANT:
		case GameObjectType::GAMEOBJECT_TYPE_BOOMERANGBRO:
		case GameObjectType::GAMEOBJECT_TYPE_DRYBONES:
			if (_index == _scores.size() - 1) {
				_score = _scores.back();
				_player->_lives += 1;
			}
			else {
				_score = _scores.at(_index);
				_player->_score += _score;
				++_index;
			}
			break;
		//1000, 2000, 4000
		case GameObjectType::GAMEOBJECT_TYPE_FORTRESSBOSS:
			_dontReset = true;

			if (_index < 6) {
				_index = 6;
			}

			if (_index == _scores.size() - 1) {
				_score = _scores.back();
				_player->_lives += 1;
			}
			else {
				_score = _scores.at(_index);
				_player->_score += _score;
				++_index;
			}
			break;
		//Everything else 10 points each
		default:
			_score = _scores.front();
			_player->_score += _score;
	}
}

bool ScorePopUp::IsFloating() const {
	return _floatStart != 0;
}

void ScorePopUp::StartFloatTimer() {
	_floatStart = static_cast<DWORD>(GetTickCount64());
}

void ScorePopUp::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_scoreTexture == nullptr) {
		_scoreTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void ScorePopUp::HandleStates() {}

void ScorePopUp::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void ScorePopUp::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	//Resets the score back to 100 when the player hits the ground
	if (_player->_isOnGround && !_dontReset) {
		_index = 2;
	}

	if (IsFloating() && GetTickCount64() - _floatStart > _floatTime) {
		_floatStart = 0;
		_position = { -1.0f, -1.0f };
	}

	GameObject::Update(deltaTime);
	_velocity.y = _gravity * deltaTime;
	_position += _distance;
}

void ScorePopUp::Render() {
	if (_score >= _scores.at(2)) {
		_animatedSprite.PlaySpriteAnimation(std::to_string(_score), _position);
	}
}

void ScorePopUp::Release() {
	_player = nullptr;
	_animatedSprite.Release();
	_scoreTexture = nullptr;
}
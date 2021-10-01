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
	//This is a bit confusing so I'll break it down for you
	//The point/score system works in a peculiar way
	//An empty brick gives 10 points, a non-empty brick gives 50 points and spawns an item
	//Coins give 50 points each
	//Red mushroom, leaf 1000 points
	//Green mushroom gives 10000 points but only adds up the player's lives, not the total score
	//Chaining enemies by stomping them or attacking them with tail or shell or fireballs gives 100, 200, 400, etc points
	//If the player touches the ground, the chain is broken and resets back to 100 points
	//Here I assume default is objectType >= GOOMBA
	//An index is used to get the current score, everytime the player defeats an enemy and is not on ground, the index increments
	//'till _index < _scores.size() - 1 where it stays until the player touches the ground
	//_score variable is used to display the current score the object emits
	switch (entity->GetObjectType()) {
		case GameObjectType::GAMEOBJECT_TYPE_SHINYBRICK:
			_score = entity->GetExtraData().empty() ? _scores.front() : _scores.at(2);
			_player->_score += _score;
			break;
		case GameObjectType::GAMEOBJECT_TYPE_COIN:
			_score = entity->GetHealth() == 1 ? _scores.at(1) : _scores.at(2);
			_player->_score += _score;
			break;
		case GameObjectType::GAMEOBJECT_TYPE_RMUSHROOM:
		case GameObjectType::GAMEOBJECT_TYPE_LEAF:
			_score = _scores.at(6);
			_player->_score += _score;
			break;
		case GameObjectType::GAMEOBJECT_TYPE_GMUSHROOM:
			_score = _scores.back();
			_player->_lives += 1;
			break;
		default:
			_score = _scores.at(_index);

			if (_player->_isOnGround) {
				_player->_score += _scores.at(2);
			}
			else {
				if (_index < _scores.size() - 1) {
					++_index;
				}

				if (_index < _scores.size() - 1) {
					_player->_score += _scores.at(_index);
				}
				else {
					_player->_lives += 1;
				}
			}
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
	if (IsFloating() && GetTickCount64() - _floatStart > _floatTime) {
		_floatStart = 0;
		_position = { -1.0f, -1.0f };
	}

	GameObject::Update(deltaTime);
	_velocity.y = _gravity * deltaTime;
	_position += _distance;
}

void ScorePopUp::Render() {
	_animatedSprite.PlaySpriteAnimation(std::to_string(_score), _position);
}

void ScorePopUp::Release() {
	_player = nullptr;
	_animatedSprite.Release();
	_scoreTexture = nullptr;
}
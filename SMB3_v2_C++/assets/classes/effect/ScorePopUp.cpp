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
	//TODO: Fix this system
	_score = 100;
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
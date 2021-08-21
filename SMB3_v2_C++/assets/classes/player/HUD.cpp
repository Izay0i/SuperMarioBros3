#include "HUD.h"

LPDIRECT3DTEXTURE9 HUD::_hudTexture = nullptr;

void HUD::_ParseLives() {
	_lives.clear();
	std::vector<unsigned int> digits = GlobalUtil::SplitDigit(10);
	//std::vector<unsigned int> digits = GlobalUtil::SplitDigit(_player->GetLives());

	for (const auto& digit : digits) {
		_lives.emplace_back(std::to_string(digit));
	}

	if (_lives.size() < _LIVES_MAX_LENGTH) {
		for (unsigned int i = 0, end = _LIVES_MAX_LENGTH - _lives.size(); i < end; ++i) {
			_lives.emplace(_lives.begin(), "0");
		}
	}
}

void HUD::_ParseCoins() {
	_coins.clear();
	std::vector<unsigned int> digits = GlobalUtil::SplitDigit(78);
	//std::vector<unsigned int> digits = GlobalUtil::SplitDigit(_player->GetCoins());

	for (const auto& digit : digits) {
		_coins.emplace_back(std::to_string(digit));
	}

	if (_coins.size() < _COINS_MAX_LENGTH) {
		for (unsigned int i = 0, end = _COINS_MAX_LENGTH - _coins.size(); i < end; ++i) {
			_coins.emplace(_coins.begin(), "0");
		}
	}
}

void HUD::_ParseItems() {
	/*
	std::vector<GameObject::GameObjectType> playerItems = _player->GetItems();
	if (playerItems.empty()) {
		return;
	}
	*/

	_items.clear();
	/*
	for (const auto& item : playerItems) {
		switch (item) {
			case GameObject::GameObjectType::GAMEOBJECT_TYPE_RMUSHROOM:
				_items.emplace_back("CardShroom");
				break;
			case GameObject::GameObjectType::GAMEOBJECT_TYPE_FLOWER:
				_items.emplace_back("CardFlower");
				break;
			case GameObject::GameObjectType::GAMEOBJECT_TYPE_STAR:
				_items.emplace_back("CardStar");
				break;
		}
	}
	*/
}

void HUD::_ParseSpeedGauge() {
	_speedGauge.clear();

	/*
	if (keyPressed || isFlying) {
		if (currentAccel >= 1.5f / maxAccel || isFlying) {
			_speedGauge.emplace_back("Arrow");
		}

		if (currentAccel >= 1.75f / maxAccel || isFlying) {
			_speedGauge.emplace_back("Arrow");
		}

		if (currentAccel >= 2.0f / maxAccel || isFlying) {
			_speedGauge.emplace_back("Arrow");
		}

		if (currentAccel >= 2.5f / maxAccel || isFlying) {
			_speedGauge.emplace_back("Arrow");
		}

		if (currentAccel >= 3.0f / maxAccel || isFlying) {
			_speedGauge.emplace_back("Arrow");
		}

		if (currentAccel >= 3.5f / maxAccel || isFlying) {
			_speedGauge.emplace_back("Arrow");
		}

		if (currentAccel >= 4.0f / maxAccel || isFlying) {
			_speedGauge.emplace_back("PButton");
		}
	}
	*/
}

void HUD::_ParseScore() {
	_score.clear();
	std::vector<unsigned int> digits = GlobalUtil::SplitDigit(700);
	//std::vector<unsigned int> digits = GlobalUtil::SplitDigit(_player->GetScore());

	for (const auto& digit : digits) {
		_score.emplace_back(std::to_string(digit));
	}

	if (_score.size() < _SCORE_MAX_LENGTH) {
		for (unsigned int i = 0, end = _SCORE_MAX_LENGTH - _score.size(); i < end; ++i) {
			_score.emplace(_score.begin(), "0");
		}
	}
}

void HUD::_ParseTimeLeft(DWORD sceneTime) {
	_timeLeft.clear();
	std::vector<unsigned int> digits = GlobalUtil::SplitDigit(sceneTime);

	for (const auto& digit : digits) {
		_timeLeft.emplace_back(std::to_string(digit));
	}

	if (_timeLeft.size() < _TIME_MAX_LENGTH) {
		for (unsigned int i = 0, end = _TIME_MAX_LENGTH - _timeLeft.size(); i < end; ++i) {
			_timeLeft.emplace(_timeLeft.begin(), "0");
		}
	}
}

void HUD::_ParseSceneEnd() {
	/*
	std::vector<GameObject::GameObjectType> playerItems = _player->GetItems();
	if (playerItems.empty()) {
		return;
	}

	GameObject::GameObjectType lastItem = playerItems.back();
	switch (lastItem) {
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_RMUSHROOM:
			_animationName = "CardShroom";
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_FLOWER:
			_animationName = "CardFlower";
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_STAR:
			_animationName = "CardStar";
			break;
	}
	*/
}

void HUD::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _hudTexture);
}

HUD::HUD(Player* player) {
	_player = player;
}

HUD::~HUD() {}

void HUD::ParseData(
	std::string dataPath, 
	const LPDIRECT3DTEXTURE9& texture, 
	std::vector<std::string> extraData) 
{
	if (_hudTexture == nullptr) {
		_hudTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void HUD::HandleStates(int, bool) {}

void HUD::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void HUD::Update(DWORD sceneTime, std::vector<GameObject*>* collidableObjects) {
	_ParseLives();
	_ParseCoins();
	_ParseItems();
	_ParseSpeedGauge();
	_ParseScore();
	_ParseTimeLeft(sceneTime);
	_ParseSceneEnd();
}

void HUD::Render() {
	_animatedSprite.PlaySpriteAnimation("BlackBG", D3DXVECTOR2(_position.x - 6.0f, _position.y - 3.0f));
	_animatedSprite.PlaySpriteAnimation("Panel", _position);

	//Lives
	for (unsigned int i = 0; i < _lives.size(); ++i) {
		_animatedSprite.PlaySpriteAnimation(_lives.at(i), D3DXVECTOR2(
			_position.x + 30.0f + (8.0f * i),
			_position.y + 16.0f
		));
	}

	//Coins
	for (unsigned int i = 0; i < _coins.size(); ++i) {
		_animatedSprite.PlaySpriteAnimation(_coins.at(i), D3DXVECTOR2(
			_position.x + 133.0f + (8.0f * i),
			_position.y + 8.0f
		));
	}

	//Items
	for (unsigned int i = 0; i < _items.size(); ++i) {
		_animatedSprite.PlaySpriteAnimation(_items.at(i), D3DXVECTOR2(
			_position.x + 161.0f + (24.0f * i),
			_position.y
		));
	}

	//Speed gauge
	for (unsigned int i = 0; i < _speedGauge.size(); ++i) {
		_animatedSprite.PlaySpriteAnimation(_speedGauge.at(i), D3DXVECTOR2(
			_position.x + 53.0f + (8.0f * i),
			_position.y + 16.0f
		));
	}

	//Score
	for (unsigned int i = 0; i < _score.size(); ++i) {
		_animatedSprite.PlaySpriteAnimation(_score.at(i), D3DXVECTOR2(
			_position.x + 53.0f + (8.0f * i),
			_position.y + 16.0f
		));
	}

	//Time left
	for (unsigned int i = 0; i < _timeLeft.size(); ++i) {
		_animatedSprite.PlaySpriteAnimation(_timeLeft.at(i), D3DXVECTOR2(
			_position.x + 125.0f + (8.0f * i),
			_position.y + 16.0f
		));
	}

	/*
	if (_player->TriggeredStageEnd()) {
		_animatedSprite.PlaySpriteAnimation("CourseClear", D3DXVECTOR2(_position.x + 60.0f, position.y - 170.0f));
		_animatedSprite.PlaySpriteAnimation(_animationName, D3DXVECTOR2(_position.x + 183.0f, _position.y - 155.0f));
	}
	*/
}

void HUD::Release() {
	_player = nullptr;
	_animatedSprite.Release();
	_hudTexture = nullptr;
}
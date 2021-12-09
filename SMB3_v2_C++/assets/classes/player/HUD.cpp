#include "../Game.h"
#include "HUD.h"

Texture* HUD::_hudTexture = nullptr;

void HUD::_ParseLives() {
	_lives.clear();
	std::vector<unsigned int> digits = GlobalUtil::SplitDigit(_player->_lives);

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
	std::vector<unsigned int> digits = GlobalUtil::SplitDigit(_player->_coins);

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
	std::vector<GameObject::GameObjectType> playerItems = _player->_bonusItems;
	if (playerItems.empty()) {
		return;
	}
	
	_items.clear();	
	for (const auto& item : playerItems) {
		switch (item) {
			case GameObject::GameObjectType::GAMEOBJECT_TYPE_REDMUSHROOM:
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
	
}

void HUD::_ParseSpeedGauge() {
	_speedGauge.clear();

	float currentAcceleration = _player->_acceleration;
	float maxAcceleration = _player->_ACCEL_THRESHOLD;
	if (_player->_isHolding || _player->IsFlying()) {
		if (currentAcceleration >= 1.5f / maxAcceleration || _player->IsFlying()) {
			_speedGauge.emplace_back("Arrow");
		}

		if (currentAcceleration >= 1.75f / maxAcceleration || _player->IsFlying()) {
			_speedGauge.emplace_back("Arrow");
		}

		if (currentAcceleration >= 2.0f / maxAcceleration || _player->IsFlying()) {
			_speedGauge.emplace_back("Arrow");
		}

		if (currentAcceleration >= 2.5f / maxAcceleration || _player->IsFlying()) {
			_speedGauge.emplace_back("Arrow");
		}

		if (currentAcceleration >= 3.0f / maxAcceleration || _player->IsFlying()) {
			_speedGauge.emplace_back("Arrow");
		}

		if (currentAcceleration >= 3.5f / maxAcceleration || _player->IsFlying()) {
			_speedGauge.emplace_back("Arrow");
		}

		if (currentAcceleration >= 4.0f / maxAcceleration || _player->IsFlying()) {
			_speedGauge.emplace_back("PButton");
		}
	}
}

void HUD::_ParseScore() {
	_score.clear();
	std::vector<unsigned int> digits = GlobalUtil::SplitDigit(_player->_score);

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
	std::vector<GameObject::GameObjectType> playerItems = _player->_bonusItems;
	if (playerItems.empty()) {
		return;
	}

	GameObject::GameObjectType lastItem = playerItems.back();
	switch (lastItem) {
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_REDMUSHROOM:
			_animationName = "CardShroom";
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_FLOWER:
			_animationName = "CardFlower";
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_STAR:
			_animationName = "CardStar";
			break;
	}
}

void HUD::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _hudTexture);
}

HUD::HUD(Player* player) {
	_player = player;
	_animationName = "NoItem";
}

HUD::~HUD() {}

void HUD::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_hudTexture == nullptr) {
		_hudTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void HUD::HandleStates() {}

void HUD::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void HUD::Update(DWORD sceneTime, std::vector<GameObject*>*) {
	_ParseLives();
	_ParseCoins();
	_ParseItems();
	_ParseSpeedGauge();
	_ParseScore();
	_ParseTimeLeft(sceneTime);
	_ParseSceneEnd();
}

void HUD::Render() {
	const float BG_OFFSET_X = 2.0f;
	const float BG_OFFSET_Y = 42.0f;
	_animatedSprite.PlaySpriteAnimation("BlackBG", { _position.x - BG_OFFSET_X, _position.y + BG_OFFSET_Y });
	_animatedSprite.PlaySpriteAnimation("Panel", _position);

	const float MARGIN_0 = 8.0f;
	const float MARGIN_1 = 24.0f;
	//Lives
	const float LIVES_OFFSET_X = 98.0f;
	const float LIVES_OFFSET_Y = 5.0f;
	for (unsigned int i = 0; i < _lives.size(); ++i) {
		_animatedSprite.PlaySpriteAnimation(_lives.at(i), D3DXVECTOR2(
			_position.x - LIVES_OFFSET_X + (MARGIN_0 * i),
			_position.y + LIVES_OFFSET_Y
		));
	}

	//Coins
	const float COINS_OFFSET_X = 6.0f;
	const float COINS_OFFSET_Y = 3.0f;
	for (unsigned int i = 0; i < _coins.size(); ++i) {
		_animatedSprite.PlaySpriteAnimation(_coins.at(i), D3DXVECTOR2(
			_position.x + COINS_OFFSET_X + (MARGIN_0 * i),
			_position.y - COINS_OFFSET_Y
		));
	}

	//Items
	const float ITEMS_OFFSET_X = 42.0f;
	for (unsigned int i = 0; i < _items.size(); ++i) {
		_animatedSprite.PlaySpriteAnimation(_items.at(i), D3DXVECTOR2(
			_position.x + ITEMS_OFFSET_X + (MARGIN_1 * i),
			_position.y
		));
	}

	//Speed gauge
	const float GAUGE_OFFSET_X = 74.0f;
	const float GAUGE_OFFSET_Y = 3.0f;
	for (unsigned int i = 0; i < _speedGauge.size(); ++i) {
		_animatedSprite.PlaySpriteAnimation(_speedGauge.at(i), D3DXVECTOR2(
			_position.x - GAUGE_OFFSET_X + (MARGIN_0 * i),
			_position.y - GAUGE_OFFSET_Y
		));
	}

	//Score
	const float SCORE_OFFSET_X = 74.0f;
	const float SCORE_OFFSET_Y = 5.0f;
	for (unsigned int i = 0; i < _score.size(); ++i) {
		_animatedSprite.PlaySpriteAnimation(_score.at(i), D3DXVECTOR2(
			_position.x - SCORE_OFFSET_X + (MARGIN_0 * i),
			_position.y + SCORE_OFFSET_Y
		));
	}

	//Time left
	const float TIME_OFFSET_X = 2.0f;
	const float TIME_OFFSET_Y = 5.0f;
	for (unsigned int i = 0; i < _timeLeft.size(); ++i) {
		_animatedSprite.PlaySpriteAnimation(_timeLeft.at(i), D3DXVECTOR2(
			_position.x - TIME_OFFSET_X + (MARGIN_0 * i),
			_position.y + TIME_OFFSET_Y
		));
	}

	if (_player->_triggeredStageEnd && !_player->_hasBossItem) {
		const float COURSE_OFFSET_X = 10.0f;
		const float COURSE_OFFSET_Y = 135.0f;
		_animatedSprite.PlaySpriteAnimation("CourseClear", D3DXVECTOR2(_position.x - COURSE_OFFSET_X, _position.y - COURSE_OFFSET_Y));
		const float ANI_OFFSET_X = 52.0f;
		const float ANI_OFFSET_Y = 126.0f;
		_animatedSprite.PlaySpriteAnimation(_animationName, D3DXVECTOR2(_position.x + ANI_OFFSET_X, _position.y - ANI_OFFSET_Y));
	}
}

void HUD::Release() {
	_player = nullptr;
	_animatedSprite.Release();
	_hudTexture = nullptr;
}
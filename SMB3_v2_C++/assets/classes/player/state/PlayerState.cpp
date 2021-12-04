#include <random>

#include "PlayerState.h"

Player* PlayerState::_player = nullptr;

void PlayerState::_OnTransform() {
	switch (_player->_health) {
		case 1:
			_player->_animatedSprite.PlaySpriteAnimation("ShrinkDown", { _player->_position.x, _player->_position.y - 8.0f }, _player->_scale);
			break;
		case 2:
			_player->_animatedSprite.PlaySpriteAnimation("GrowUp", { _player->_position.x, _player->_position.y - 8.0f }, _player->_scale);
			break;
		case 3:
		case 4:
			_player->_animatedSprite.PlaySpriteAnimation("SmokePuff", _player->_position);
			break;
	}
}

PlayerState::PlayerState(Player* player) {
	if (_player == nullptr) {
		_player = player;
		_currentHealth = _player->_health;
	}
}

PlayerState::~PlayerState() {}

void PlayerState::Update(DWORD) {
	if (_player->isInMap && GetTickCount64() % 50 == 0) {		
		_player->_scale.x = -_player->_scale.x;
	}

	if (_player->IsInvulnerable()) {
		std::random_device device;
		std::mt19937 rng(device());
		std::uniform_int_distribution<std::mt19937::result_type> dist(63, 128);

		_alpha = dist(rng);
	}
	else {
		_alpha = 255;
	}

	_form = static_cast<_Form>(_player->_health);
}

void PlayerState::Render() {
	if (_player->IsInvulnerable()) {
		_OnTransform();
	}

	if (_player->_health == 0) {
		_player->_animatedSprite.PlaySpriteAnimation("Die", _player->_position);
	}
}

void PlayerState::Release() {
	_player = nullptr;
}
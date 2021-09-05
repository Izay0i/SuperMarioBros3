#include "PlayerState.h"

Player* PlayerState::_player = nullptr;

void PlayerState::_OnEntry() {
	switch (_player->_health) {
		case 2:
			_player->_animatedSprite.PlaySpriteAnimation("GrowUp", _player->_position, _player->_scale);
			break;
		case 3:
		case 4:
			_player->_animatedSprite.PlaySpriteAnimation("SmokePuff", _player->_position);
			break;
	}
}

void PlayerState::_OnExit() {
	switch (_player->_health) {
		case 1:
			_player->_animatedSprite.PlaySpriteAnimation("ShrinkDown", _player->_position, _player->_scale);
			break;
		case 2:
			_player->_animatedSprite.PlaySpriteAnimation("SmokePuff", _player->_position);
			break;
	}
}

PlayerState::~PlayerState() {}

void PlayerState::Update(DWORD) {
	if (_player->IsInvulnerable()) {
		_currentHealth = _player->_health;
	}

	switch (_player->_health) {
		case 1:
			_form = _Form::SMALL;
			break;
		case 2:
			_form = _Form::BIG;
			break;
		case 3:
			_form = _Form::FIRE;
			break;
		case 4:
			_form = _Form::RACCOON;
			break;
	}
}

void PlayerState::Render() {
	if (_player->IsInvulnerable()) {
		if (_currentHealth > _player->_health) {
			_OnExit();
		}
		else {
			_OnEntry();
		}
		return;
	}

	if (_player->_health == 0) {
		_player->_animatedSprite.PlaySpriteAnimation("Die", _player->_position);
		return;
	}
}

void PlayerState::Release() {
	_player = nullptr;
}
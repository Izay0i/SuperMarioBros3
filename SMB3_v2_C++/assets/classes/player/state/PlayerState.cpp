#include "PlayerState.h"

Player* PlayerState::_player = nullptr;

PlayerState::~PlayerState() {}

void PlayerState::Update(DWORD) {
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
	if (_player->_health == 0) {
		_player->_animatedSprite.PlaySpriteAnimation("Die", _player->_position);
		return;
	}
}

void PlayerState::Release() {
	_player = nullptr;
}
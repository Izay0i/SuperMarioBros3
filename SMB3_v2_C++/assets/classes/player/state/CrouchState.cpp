#include "../../Device.h"
#include "PlayerState.h"
#include "IdleState.h"
#include "CrouchState.h"

CrouchState::CrouchState(Player* player) {
	if (_player == nullptr) {
		_player = player;
	}
}

PlayerState* CrouchState::HandleStates() {
	if (_player->_velocity.x != 0.0f || !Device::IsKeyDown(DIK_S)) {
		return new IdleState(_player);
	}

	return nullptr;
}

void CrouchState::Render() {
	switch (_form) {
		case _Form::SMALL:
			if (_player->IsInPipe()) {
				_player->_animatedSprite.PlaySpriteAnimation("Front", _player->_position);
			}
			else {
				_player->_animatedSprite.PlaySpriteAnimation("Idle", _player->_position, _player->_scale);
			}
			break;
		case _Form::BIG:
			if (_player->IsInPipe()) {
				_player->_animatedSprite.PlaySpriteAnimation("BigFront", _player->_position);
			}
			else {
				_player->_animatedSprite.PlaySpriteAnimation("BigCrouch", _player->_position, _player->_scale);
			}
			break;
		case _Form::FIRE:
			if (_player->IsInPipe()) {
				_player->_animatedSprite.PlaySpriteAnimation("FireFront", _player->_position);
			}
			else {
				_player->_animatedSprite.PlaySpriteAnimation("FireCrouch", _player->_position, _player->_scale);
			}
			break;
		case _Form::RACCOON:
			if (_player->IsInPipe()) {
				_player->_animatedSprite.PlaySpriteAnimation("RacFront", _player->_position);
			}
			else {
				_player->_animatedSprite.PlaySpriteAnimation("RacCrouch", _player->_position, _player->_scale);
			}
			break;
	}
}
#include "../../Device.h"
#include "PlayerState.h"
#include "IdleState.h"
#include "CrouchState.h"

CrouchState::CrouchState(Player* player) : PlayerState(player) {}

PlayerState* CrouchState::HandleStates() {
	if (!_player->_isCrouching) {
		return new IdleState(_player);
	}

	return nullptr;
}

void CrouchState::Render() {
	PlayerState::Render();
	if (_player->IsInvulnerable()) {
		return;
	}

	const float RAC_OFFSET = 4.0f;
	const float CROUCH_OFFSET = 8.0f;
	switch (_form) {
		case _Form::SMALL:
			if (_player->IsInPipe()) {
				_player->_animatedSprite.PlaySpriteAnimation("Front", _player->_position);
			}
			else {
				_player->_animatedSprite.PlaySpriteAnimation("Idle", _player->_position, _player->_scale, _alpha);
			}
			break;
		case _Form::BIG:
			if (_player->IsInPipe()) {
				_player->_animatedSprite.PlaySpriteAnimation("BigFront", _player->_position);
			}
			else {
				_player->_animatedSprite.PlaySpriteAnimation("BigCrouch", { _player->_position.x, _player->_position.y - CROUCH_OFFSET }, _player->_scale, _alpha);
			}
			break;
		case _Form::FIRE:
			if (_player->IsInPipe()) {
				_player->_animatedSprite.PlaySpriteAnimation("FireFront", _player->_position);
			}
			else {
				_player->_animatedSprite.PlaySpriteAnimation("FireCrouch", { _player->_position.x, _player->_position.y - CROUCH_OFFSET }, _player->_scale, _alpha);
			}
			break;
		case _Form::RACCOON:
			if (_player->IsInPipe()) {
				_player->_animatedSprite.PlaySpriteAnimation("RacFront", _player->_position);
			}
			else {
				_player->_animatedSprite.PlaySpriteAnimation("RacCrouch", { 
					_player->_position.x - RAC_OFFSET * _player->_normal.x, 
					_player->_position.y - CROUCH_OFFSET 
					}, 
					_player->_scale, 
					_alpha
				);
			}
			break;
	}
}
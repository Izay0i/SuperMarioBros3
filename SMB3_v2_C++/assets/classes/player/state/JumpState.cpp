#include "../../Device.h"
#include "PlayerState.h"
#include "JumpState.h"
#include "FallState.h"
#include "WagState.h"

JumpState::JumpState(Player* player) {
	if (_player == nullptr) {
		_player = player;
	}
}

PlayerState* JumpState::HandleStates() {
	if (_player->_velocity.y > 0.0f) {
		return new FallState(_player);
	}
	else if (_player->IsAttacking()) {
		return new WagState(_player);
	}
	
	return nullptr;
}

void JumpState::Render() {
	PlayerState::Render();
	if (_player->IsInvulnerable()) {
		return;
	}

	switch (_form) {
		case _Form::SMALL:
			if (_player->IsInPipe()) {
				_player->_animatedSprite.PlaySpriteAnimation("Front", _player->_position);
			}
			else if (_player->_acceleration >= _player->_ACCEL_THRESHOLD && _player->_heldEntity == nullptr) {
				_player->_animatedSprite.PlaySpriteAnimation("SuperJump", _player->_position, _player->_scale);
			}
			else if (_player->_isNextToShell) {
				_player->_animatedSprite.PlaySpriteAnimation("Kick", _player->_position, _player->_scale);
			}
			else if (_player->_heldEntity != nullptr) {
				_player->_animatedSprite.PlaySpriteAnimation("HoldJump", _player->_position, _player->_scale);
			}
			else {
				_player->_animatedSprite.PlaySpriteAnimation("Jump", _player->_position, _player->_scale);
			}
			break;
		case _Form::BIG:
			if (_player->IsInPipe()) {
				_player->_animatedSprite.PlaySpriteAnimation("BigFront", _player->_position);
			}
			else if (_player->_acceleration >= _player->_ACCEL_THRESHOLD && _player->_heldEntity == nullptr) {
				_player->_animatedSprite.PlaySpriteAnimation("BigSuperJump", _player->_position, _player->_scale);
			}
			else if (_player->_isNextToShell) {
				_player->_animatedSprite.PlaySpriteAnimation("BigKick", _player->_position, _player->_scale);
			}
			else if (_player->_heldEntity != nullptr) {
				_player->_animatedSprite.PlaySpriteAnimation("BigHoldJump", _player->_position, _player->_scale);
			}
			else {
				_player->_animatedSprite.PlaySpriteAnimation("BigJump", _player->_position, _player->_scale);
			}
			break;
		case _Form::FIRE:
			if (_player->IsInPipe()) {
				_player->_animatedSprite.PlaySpriteAnimation("FireFront", _player->_position);
			}
			else if (_player->_acceleration >= _player->_ACCEL_THRESHOLD && _player->_heldEntity == nullptr) {
				_player->_animatedSprite.PlaySpriteAnimation("FireSuperJump", _player->_position, _player->_scale);
			}
			else if (_player->_isNextToShell) {
				_player->_animatedSprite.PlaySpriteAnimation("FireKick", _player->_position, _player->_scale);
			}
			else if (_player->_heldEntity != nullptr) {
				_player->_animatedSprite.PlaySpriteAnimation("FireHoldJump", _player->_position, _player->_scale);
			}
			else {
				_player->_animatedSprite.PlaySpriteAnimation("FireJump", _player->_position, _player->_scale);
			}
			break;
		case _Form::RACCOON:
			if (_player->IsInPipe()) {
				_player->_animatedSprite.PlaySpriteAnimation("RacFront", _player->_position);
			}
			else if (_player->_heldEntity == nullptr) {
				if (_player->_acceleration >= _player->_ACCEL_THRESHOLD) {
					_player->_animatedSprite.PlaySpriteAnimation("RacSuperJump", _player->_position, _player->_scale);
				}
				else if (_player->IsFlying() && Device::IsKeyDown(DIK_K)) {
					_player->_animatedSprite.PlaySpriteAnimation("RacWagSuperFall", _player->_position, _player->_scale);
				}
				else {
					_player->_animatedSprite.PlaySpriteAnimation("RacJump", _player->_position, _player->_scale);
				}
			}
			else if (_player->_isNextToShell) {
				_player->_animatedSprite.PlaySpriteAnimation("RacKick", _player->_position, _player->_scale);
			}
			else if (_player->_heldEntity != nullptr) {
				_player->_animatedSprite.PlaySpriteAnimation("RacHoldJump", _player->_position, _player->_scale);
			}
			break;
	}
}
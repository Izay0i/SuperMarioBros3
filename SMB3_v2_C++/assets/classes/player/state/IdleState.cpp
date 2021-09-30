#include "../../Device.h"
#include "PlayerState.h"
#include "IdleState.h"
#include "RunState.h"
#include "JumpState.h"
#include "FallState.h"
#include "CrouchState.h"
#include "ThrowState.h"
#include "WagState.h"

IdleState::IdleState(Player* player) {
	if (_player == nullptr) {
		_player = player;
		_currentHealth = _player->_health;
	}
}

PlayerState* IdleState::HandleStates() {
	if (_player->_velocity.x != 0.0f && _player->_isOnGround || _player->_triggeredStageEnd) {
		return new RunState(_player);
	}
	else if (_player->_velocity.y < 0.0f) {
		return new JumpState(_player);
	}
	else if (_player->_velocity.y > 0.0f && !_player->_isOnGround) {
		return new FallState(_player);
	}
	else if (_player->_heldEntity == nullptr) {
		if (_player->_isCrouching) {
			return new CrouchState(_player);
		}
		else if (Device::IsKeyDown(DIK_J) && _form == _Form::FIRE) {
			return new ThrowState(_player);
		}
		else if (_player->IsAttacking() && _form == _Form::RACCOON) {
			return new WagState(_player);
		}
	}

	return nullptr;
}

void IdleState::Render() {
	PlayerState::Render();

	switch (_form) {
		case _Form::SMALL:
			if (_player->IsInPipe()) {
				_player->_animatedSprite.PlaySpriteAnimation("Front", _player->_position);
			}
			else if (_player->_isNextToShell) {
				_player->_animatedSprite.PlaySpriteAnimation("Kick", _player->_position, _player->_scale);
			}
			else if (_player->_heldEntity != nullptr) {
				_player->_animatedSprite.PlaySpriteAnimation("HoldIdle", _player->_position, _player->_scale);
			}
			else {
				_player->_animatedSprite.PlaySpriteAnimation("Idle", _player->_position, _player->_scale);
			}
			break;
		case _Form::BIG:
			if (_player->IsInPipe()) {
				_player->_animatedSprite.PlaySpriteAnimation("BigFront", _player->_position);
			}
			else if (_player->_isNextToShell) {
				_player->_animatedSprite.PlaySpriteAnimation("BigKick", _player->_position, _player->_scale);
			}
			else if (_player->_heldEntity != nullptr) {
				_player->_animatedSprite.PlaySpriteAnimation("BigHoldIdle", _player->_position, _player->_scale);
			}
			else {
				_player->_animatedSprite.PlaySpriteAnimation("BigIdle", _player->_position, _player->_scale);
			}
			break;
		case _Form::FIRE:
			if (_player->IsInPipe()) {
				_player->_animatedSprite.PlaySpriteAnimation("FireFront", _player->_position);
			}
			else if (_player->_isNextToShell) {
				_player->_animatedSprite.PlaySpriteAnimation("FireKick", _player->_position, _player->_scale);
			}
			else if (_player->_heldEntity != nullptr) {
				_player->_animatedSprite.PlaySpriteAnimation("FireHoldIdle", _player->_position, _player->_scale);
			}
			else {
				_player->_animatedSprite.PlaySpriteAnimation("FireIdle", _player->_position, _player->_scale);
			}
			break;
		case _Form::RACCOON:
			if (_player->IsInPipe()) {
				_player->_animatedSprite.PlaySpriteAnimation("RacFront", _player->_position);
			}
			else if (_player->_isNextToShell) {
				_player->_animatedSprite.PlaySpriteAnimation("RacKick", _player->_position, _player->_scale);
			}
			else if (_player->_heldEntity != nullptr) {
				_player->_animatedSprite.PlaySpriteAnimation("RacHoldIdle", _player->_position, _player->_scale);
			}
			else {
				_player->_animatedSprite.PlaySpriteAnimation("RacIdle", _player->_position, _player->_scale);
			}
			break;
	}
}
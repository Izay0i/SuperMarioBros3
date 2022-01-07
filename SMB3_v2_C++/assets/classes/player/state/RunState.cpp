#include "../../Device.h"
#include "PlayerState.h"
#include "IdleState.h"
#include "RunState.h"
#include "JumpState.h"
#include "FallState.h"
#include "CrouchState.h"
#include "WagState.h"

RunState::RunState(Player* player) : PlayerState(player) {}

PlayerState* RunState::HandleStates() {
	if (_player->_velocity.x == 0.0f) {
		return new IdleState(_player);
	}
	else if (_player->_velocity.y < 0.0f) {
		return new JumpState(_player);
	}
	else if (_player->_velocity.y > 0.0f && !_player->_isOnGround) {
		return new FallState(_player);
	}
	else if (_player->_isCrouching) {
		return new CrouchState(_player);
	}
	else if (_player->IsAttacking()) {
		return new WagState(_player);
	}

	return nullptr;
}

void RunState::Render() {
	PlayerState::Render();
	if (_player->IsInvulnerable()) {
		return;
	}

	const float RAC_OFFSET = 4.0f;
	switch (_form) {
		case _Form::SMALL:
			if (_player->IsInPipe()) {
				_player->_animatedSprite.PlaySpriteAnimation("Front", _player->_position);
			}
			else if (_player->_acceleration >= _player->_ACCEL_THRESHOLD && _player->_isOnGround && _player->_heldEntity == nullptr) {
				_player->_animatedSprite.PlaySpriteAnimation("SuperRun", _player->_position, _player->_scale, _alpha);
			}
			else if (_player->_isNextToShell) {
				_player->_animatedSprite.PlaySpriteAnimation("Kick", _player->_position, _player->_scale, _alpha);
			}
			else if (_player->_heldEntity != nullptr) {
				_player->_animatedSprite.PlaySpriteAnimation("HoldRun", _player->_position, _player->_scale, _alpha);
			}
			else {
				if (_player->_acceleration < 0.5f && (Device::IsKeyDown(DIK_LEFTARROW) || Device::IsKeyDown(DIK_RIGHTARROW))) {
					_player->_animatedSprite.PlaySpriteAnimation("Skid", _player->_position, _player->_scale, _alpha);
				}
				else {
					_player->_animatedSprite.PlaySpriteAnimation("Run", _player->_position, _player->_scale, _alpha);
				}
			}
			break;
		case _Form::BIG:
			if (_player->IsInPipe()) {
				_player->_animatedSprite.PlaySpriteAnimation("BigFront", _player->_position);
			}
			else if (_player->_acceleration >= _player->_ACCEL_THRESHOLD && _player->_isOnGround && _player->_heldEntity == nullptr) {
				_player->_animatedSprite.PlaySpriteAnimation("BigSuperRun", _player->_position, _player->_scale, _alpha);
			}
			else if (_player->_isNextToShell) {
				_player->_animatedSprite.PlaySpriteAnimation("BigKick", _player->_position, _player->_scale, _alpha);
			}
			else if (_player->_heldEntity != nullptr) {
				_player->_animatedSprite.PlaySpriteAnimation("BigHoldRun", _player->_position, _player->_scale, _alpha);
			}
			else {
				if (_player->_acceleration < 0.5f && (Device::IsKeyDown(DIK_LEFTARROW) || Device::IsKeyDown(DIK_RIGHTARROW))) {
					_player->_animatedSprite.PlaySpriteAnimation("BigSkid", _player->_position, _player->_scale, _alpha);
				}
				else {
					_player->_animatedSprite.PlaySpriteAnimation("BigRun", _player->_position, _player->_scale, _alpha);
				}
			}
			break;
		case _Form::FIRE:
			if (_player->IsInPipe()) {
				_player->_animatedSprite.PlaySpriteAnimation("FireFront", _player->_position);
			}
			else if (_player->_acceleration >= _player->_ACCEL_THRESHOLD && _player->_isOnGround && _player->_heldEntity == nullptr) {
				_player->_animatedSprite.PlaySpriteAnimation("FireSuperRun", _player->_position, _player->_scale, _alpha);
			}
			else if (_player->_isNextToShell) {
				_player->_animatedSprite.PlaySpriteAnimation("FireKick", _player->_position, _player->_scale, _alpha);
			}
			else if (_player->_heldEntity != nullptr) {
				_player->_animatedSprite.PlaySpriteAnimation("FireHoldRun", _player->_position, _player->_scale, _alpha);
			}
			else {
				if (_player->_acceleration < 0.5f && (Device::IsKeyDown(DIK_LEFTARROW) || Device::IsKeyDown(DIK_RIGHTARROW))) {
					_player->_animatedSprite.PlaySpriteAnimation("FireSkid", _player->_position, _player->_scale, _alpha);
				}
				else {
					_player->_animatedSprite.PlaySpriteAnimation("FireRun", _player->_position, _player->_scale, _alpha);
				}
			}
			break;
		case _Form::RACCOON:
			if (_player->IsInPipe()) {
				_player->_animatedSprite.PlaySpriteAnimation("RacFront", _player->_position);
			}
			else if (_player->_acceleration >= _player->_ACCEL_THRESHOLD && _player->_isOnGround && _player->_heldEntity == nullptr) {
				_player->_animatedSprite.PlaySpriteAnimation("RacSuperRun", { 
					_player->_position.x - RAC_OFFSET * _player->_normal.x,  
					_player->_position.y 
					}, 
					_player->_scale, 
					_alpha
				);
			}
			else if (_player->_isNextToShell) {
				_player->_animatedSprite.PlaySpriteAnimation("RacKick", { 
					_player->_position.x - RAC_OFFSET * _player->_normal.x,  
					_player->_position.y 
					}, 
					_player->_scale, 
					_alpha
				);
			}
			else if (_player->_heldEntity != nullptr) {
				_player->_animatedSprite.PlaySpriteAnimation("RacHoldRun", { 
					_player->_position.x - RAC_OFFSET * _player->_normal.x,  
					_player->_position.y 
					}, 
					_player->_scale, 
					_alpha
				);
			}
			else {
				if (_player->_acceleration < 0.5f && (Device::IsKeyDown(DIK_LEFTARROW) || Device::IsKeyDown(DIK_RIGHTARROW))) {
					_player->_animatedSprite.PlaySpriteAnimation("RacSkid", _player->_position, _player->_scale, _alpha);
				}
				else {
					_player->_animatedSprite.PlaySpriteAnimation("RacRun", { 
						_player->_position.x - RAC_OFFSET * _player->_normal.x,  
						_player->_position.y 
						}, 
						_player->_scale, 
						_alpha
					);
				}
			}
			break;
	}
}
#include "BossState.h"
#include "BossJumpState.h"
#include "BossRunState.h"

BossJumpState::BossJumpState(FortressBoss* fortressBoss) : BossState(fortressBoss) {}

BossState* BossJumpState::HandleStates() {
	if (_fortressBoss->_isOnGround) {
		return new BossRunState(_fortressBoss);
	}

	return nullptr;
}

void BossJumpState::Update(DWORD) {
	_fortressBoss->_velocity.x = _fortressBoss->_runSpeed * _fortressBoss->_normal.x;
	_fortressBoss->_velocity.y = -_fortressBoss->_jumpSpeed;
}

void BossJumpState::Render() {
	_fortressBoss->_animatedSprite.PlaySpriteAnimation("Jump", _fortressBoss->_position);
}
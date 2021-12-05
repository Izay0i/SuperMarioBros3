#include "BossState.h"
#include "BossIdleState.h"
#include "BossJumpState.h"

BossIdleState::BossIdleState(FortressBoss* fortressBoss) : BossState(fortressBoss) {}

BossState* BossIdleState::HandleStates() {
	if (_fortressBoss->commenceBattle) {
		return new BossJumpState(_fortressBoss);
	}

	return nullptr;
}

void BossIdleState::Render() {
	_fortressBoss->_animatedSprite.PlaySpriteAnimation("Idle", _fortressBoss->_position);
}
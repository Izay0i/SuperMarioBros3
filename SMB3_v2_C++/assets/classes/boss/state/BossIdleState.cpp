#include "BossState.h"
#include "BossIntroState.h"
#include "BossIdleState.h"

BossIdleState::BossIdleState(FortressBoss* fortressBoss) : BossState(fortressBoss) {}

BossState* BossIdleState::HandleStates() {
	if (_fortressBoss->commenceBattle) {
		return new BossIntroState(_fortressBoss);
	}

	return nullptr;
}

void BossIdleState::Render() {
	_fortressBoss->_animatedSprite.PlaySpriteAnimation("Idle", _fortressBoss->_position);
}
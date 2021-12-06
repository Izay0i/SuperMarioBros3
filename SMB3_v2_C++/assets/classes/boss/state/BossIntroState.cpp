#include "BossState.h"
#include "BossIntroState.h"
#include "BossRunState.h"

BossIntroState::BossIntroState(FortressBoss* fortressBoss) : BossState(fortressBoss) {}

BossState* BossIntroState::HandleStates() {
	if (!_fortressBoss->IsInIntro()) {
		return new BossRunState(_fortressBoss);
	}

	return nullptr;
}

void BossIntroState::Render() {
	_fortressBoss->_animatedSprite.PlaySpriteAnimation("GetUp", _fortressBoss->_position);
}
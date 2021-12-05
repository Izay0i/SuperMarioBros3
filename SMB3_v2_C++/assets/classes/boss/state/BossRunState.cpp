#include "BossState.h"
#include "BossRunState.h"
#include "BossAttackState.h"
#include "BossHurtState.h"

BossRunState::BossRunState(FortressBoss* fortressBoss) : BossState(fortressBoss) {}

BossState* BossRunState::HandleStates() {
	if (!_fortressBoss->IsOnCoolDown()) {
		return new BossAttackState(_fortressBoss);
	}
	else if (_fortressBoss->IsInvulnerable()) {
		return new BossHurtState(_fortressBoss);
	}

	return nullptr;
}

void BossRunState::Update(DWORD deltaTime) {
	_fortressBoss->_velocity.x = _fortressBoss->_runSpeed * _fortressBoss->_normal.x;
}

void BossRunState::Render() {
	_fortressBoss->_animatedSprite.PlaySpriteAnimation("Run", _fortressBoss->_position);
}
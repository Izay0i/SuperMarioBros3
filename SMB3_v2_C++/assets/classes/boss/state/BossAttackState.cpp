#include "BossState.h"
#include "BossRunState.h"
#include "BossAttackState.h"

BossAttackState::BossAttackState(FortressBoss* fortressBoss) : BossState(fortressBoss) {}

BossState* BossAttackState::HandleStates() {
	if (_fortressBoss->IsOnCoolDown()) {
		return new BossRunState(_fortressBoss);
	}

	return nullptr;
}

void BossAttackState::Update(DWORD deltaTime) {
	switch (_fortressBoss->_health) {
		case 3:
			_fortressBoss->_velocity.x = 0.0f;
			break;
		case 2:
			_fortressBoss->_velocity.y = -_fortressBoss->_jumpSpeed;
			break;
		case 1:
			_fortressBoss->_velocity.x = _fortressBoss->_runSpeed * _fortressBoss->_normal.x * 2.0f;
			break;
	}
}

void BossAttackState::Render() {
	switch (_fortressBoss->_health) {
		case 3:
			_fortressBoss->_animatedSprite.PlaySpriteAnimation("Idle", _fortressBoss->_position);
			break;
		case 2:
			_fortressBoss->_animatedSprite.PlaySpriteAnimation("Jump", _fortressBoss->_position);
			break;
		case 1:
			_fortressBoss->_animatedSprite.PlaySpriteAnimation("Run", _fortressBoss->_position);
			break;
	}
}
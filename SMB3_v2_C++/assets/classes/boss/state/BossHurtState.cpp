#include "BossState.h"
#include "BossRunState.h"
#include "BossHurtState.h"

BossHurtState::BossHurtState(FortressBoss* fortressBoss) : BossState(fortressBoss) {}

BossState* BossHurtState::HandleStates() {
	if (!_fortressBoss->IsInvulnerable()) {
		return new BossRunState(_fortressBoss);
	}

	return nullptr;
}

void BossHurtState::Update(DWORD deltaTime) {
	_fortressBoss->_velocity.x = 0.0f;
	if (_fortressBoss->_isOnGround) {
		_fortressBoss->_velocity.y = 0.0f;
	}
	_fortressBoss->_scale.x = -_fortressBoss->_scale.x;
}

void BossHurtState::Render() {
	_fortressBoss->_animatedSprite.PlaySpriteAnimation(
		_fortressBoss->_health <= 0 ? "Dead" : "Hurt", 
		_fortressBoss->_position, 
		_fortressBoss->_scale
	);
}
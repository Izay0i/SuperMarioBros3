#include "BossState.h"
#include "BossRunState.h"
#include "BossHurtState.h"

BossHurtState::BossHurtState(FortressBoss* fortressBoss) : BossState(fortressBoss), _scaleX(1.0f) {}

BossState* BossHurtState::HandleStates() {
	if (!_fortressBoss->IsInvulnerable()) {
		return new BossRunState(_fortressBoss);
	}

	return nullptr;
}

void BossHurtState::Update(DWORD deltaTime) {
	_fortressBoss->_velocity.x = 0.0f;
	
	if (GetTickCount64() % 50 == 0) {
		_scaleX = -_scaleX;
	}
}

void BossHurtState::Render() {
	_fortressBoss->_animatedSprite.PlaySpriteAnimation(
		_fortressBoss->_health <= 0 ? "Dead" : "Hurt", 
		_fortressBoss->_position, 
		{ _scaleX, _fortressBoss->_scale.y }
	);
}
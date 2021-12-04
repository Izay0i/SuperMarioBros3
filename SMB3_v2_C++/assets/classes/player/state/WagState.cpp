#include "../../Device.h"
#include "PlayerState.h"
#include "IdleState.h"
#include "WagState.h"

WagState::WagState(Player* player) : PlayerState(player) {}

PlayerState* WagState::HandleStates() {
	if (!_player->IsAttacking()) {
		return new IdleState(_player);
	}

	return nullptr;
}

void WagState::Render() {
	if (_player->IsInPipe()) {
		_player->_animatedSprite.PlaySpriteAnimation("RacFront", _player->_position);
	}
	else if (_player->IsAttacking()) {
		_player->_animatedSprite.PlaySpriteAnimation("RacWag", _player->_position, D3DXVECTOR2(_player->_normal.x, 1.0f), _alpha);
	}
}
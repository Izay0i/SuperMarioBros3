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
	const float RAC_OFFSET = 4.0f;
	if (_player->IsInPipe()) {
		_player->_animatedSprite.PlaySpriteAnimation("RacFront", _player->_position);
	}
	else if (_player->IsAttacking()) {
		_player->_animatedSprite.PlaySpriteAnimation("RacWag", { 
			_player->_position.x - RAC_OFFSET * _player->_normal.x,  
			_player->_position.y 
			}, 
			{ _player->_normal.x, _player->_scale.y },
			_alpha
		);
	}
}
#include "../../Device.h"
#include "PlayerState.h"
#include "IdleState.h"
#include "ThrowState.h"

ThrowState::ThrowState(Player* player) : PlayerState(player) {}

PlayerState* ThrowState::HandleStates() {
	const int B_KEY = Device::GetInstance()->GetControllerKey("B");

	if (_player->_velocity.x != 0.0f || !Device::IsKeyDown(B_KEY)) {
		return new IdleState(_player);
	}

	return nullptr;
}

void ThrowState::Render() {
	if (_player->IsInPipe()) {
		_player->_animatedSprite.PlaySpriteAnimation("FireFront", _player->_position);
	}
	else {
		_player->_animatedSprite.PlaySpriteAnimation("FireThrow", _player->_position, _player->_scale, _alpha);
	}
}
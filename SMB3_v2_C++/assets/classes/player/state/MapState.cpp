#include "PlayerState.h"
#include "MapState.h"
#include "IdleState.h"

MapState::MapState(Player* player) : PlayerState(player) {}

PlayerState* MapState::HandleStates() {
	if (!_player->isInMap) {
		return new IdleState(_player);
	}
	
	return nullptr;
}

void MapState::Render() {
	const float MAP_OFFSET = 3.0f;
	switch (_form) {
		case _Form::SMALL:
			_player->_animatedSprite.PlaySpriteAnimation("MapSmall", { _player->_position.x, _player->_position.y - MAP_OFFSET }, _player->_scale);
			break;
		case _Form::BIG:
			_player->_animatedSprite.PlaySpriteAnimation("MapBig", { _player->_position.x, _player->_position.y - MAP_OFFSET }, _player->_scale);
			break;
		case _Form::FIRE:
			_player->_animatedSprite.PlaySpriteAnimation("MapFire", { _player->_position.x, _player->_position.y - MAP_OFFSET }, _player->_scale);
			break;
		case _Form::RACCOON:
			_player->_animatedSprite.PlaySpriteAnimation("MapRac", { _player->_position.x, _player->_position.y - MAP_OFFSET }, _player->_scale);
			break;
	}
}

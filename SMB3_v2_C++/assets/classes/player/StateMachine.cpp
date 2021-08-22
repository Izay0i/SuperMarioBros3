#include "StateMachine.h"

void StateMachine::_HandleForms() {
	switch (_player->_health) {
		case 1:
			_form = _Form::SMALL;
			break;
		case 2:
			_form = _Form::BIG;
			break;
		case 3:
			_form = _Form::FIRE;
			break;
		case 4:
			_form = _Form::RACCOON;
			break;
	}
}

void StateMachine::_OnEntry() {
	switch (_currentHealth) {
		case 1:
			_player->_animatedSprite.PlaySpriteAnimation("GrowUp", _player->_position);
			break;
		case 2:
			_player->_animatedSprite.PlaySpriteAnimation("SmokePuff", _player->_position);
			break;
		case 3:
			_player->_animatedSprite.PlaySpriteAnimation("SmokePuff", _player->_position);
			break;
		case 4:
			_player->_animatedSprite.PlaySpriteAnimation("SmokePuff", _player->_position);
			break;
	}
}

void StateMachine::_OnExit() {
	switch (_player->_health) {
		case 1:
			_player->_animatedSprite.PlaySpriteAnimation("ShrinkDown", _player->_position);
			break;
		case 2:
			_player->_animatedSprite.PlaySpriteAnimation("SmokePuff", _player->_position);
			break;
		case 3:
			_player->_animatedSprite.PlaySpriteAnimation("SmokePuff", _player->_position);
			break;
		case 4:
			_player->_animatedSprite.PlaySpriteAnimation("SmokePuff", _player->_position);
			break;
	}
}

StateMachine::StateMachine(Player* player) {
	_scaleX = 1.0f;
	_alpha = 255;
	_currentHealth = player->_health;
	_player = player;

	_HandleForms();
}

StateMachine::~StateMachine() {}

void StateMachine::HandleStates(int keyCode, bool isKeyDown) {	
	if (_currentHealth != _player->_health) {
		if (_currentHealth < _player->_health) {
			_OnEntry();
		}
		else if (_currentHealth > _player->_health) {
			_OnExit();
		}
		
		_currentHealth = _player->_health;
		_HandleForms();
	}	

	GlobalUtil::KeyInput keyInput = static_cast<GlobalUtil::KeyInput>(keyCode);
	switch (_state) {
		case _State::MAP:
			
			break;
		case _State::IDLE:
			if (_player->_velocity.x != 0.0f && _player->_isOnGround) {
				_state = _State::RUN;
			}
			else if (_player->_velocity.y < 0.0f) {
				_state = _State::JUMP;
			}
			else if (_player->_velocity.y > 0.0f && !_player->_isOnGround) {
				_state = _State::FALL;
			}
			else if (_player->_heldEntity == nullptr) {
				if (isKeyDown) {
					if (keyInput == GlobalUtil::KeyInput::KEY_INPUT_DOWN) {
						_state = _State::CROUCH;
					}
					else if (keyInput == GlobalUtil::KeyInput::KEY_INPUT_B && _form == _Form::FIRE) {
						_state = _State::THROW;
					}
					/*
					else if (_player->IsAttacking()) {
						_state = _State::WAG;
					}
					*/
				}
			}
			break;
		case _State::RUN:
			if (_player->_velocity.x == 0.0f) {
				_state = _State::IDLE;
			}
			else if (_player->_velocity.y < 0.0f) {
				_state = _State::JUMP;
			}
			else if (_player->_velocity.y > 0.0f && !_player->_isOnGround) {
				_state = _State::FALL;
			}
			break;
		case _State::JUMP:
			if (_player->_velocity.y > 0.0f) {
				_state = _State::FALL;
			}
			/*
			else if (_player->IsAttacking()) {
				_state = _State::WAG;
			}
			*/
			break;
		case _State::FALL:
			if (_player->_isOnGround) {
				_state = _State::IDLE;
			}
			/*
			else if (_player->IsAttacking()) {
				_state = _State::WAG;
			}
			*/
			break;
		case _State::CROUCH:
			if (_player->_velocity.x != 0.0f || 
				(!isKeyDown && keyInput == GlobalUtil::KeyInput::KEY_INPUT_DOWN)
				) 
			{
				_state = _State::IDLE;
			}
			break;
		case _State::THROW:
			if (_player->_velocity.x != 0.0f || 
				(!isKeyDown && keyInput == GlobalUtil::KeyInput::KEY_INPUT_B)) 
			{
				_state = _State::IDLE;
			}
			break;
		case _State::WAG:
			if (_player->_velocity.x != 0.0f || 
				(!isKeyDown && keyInput == GlobalUtil::KeyInput::KEY_INPUT_B)) 
			{
				_state = _State::IDLE;
			}
			break;
		default:
			_state = _State::IDLE;
	}
}

void StateMachine::Update(DWORD delta) {

}

void StateMachine::Render() {
	if (_player->_health == 0) {
		_player->_animatedSprite.PlaySpriteAnimation("Die", _player->_position);
		return;
	}

	switch (_state) {
		case _State::MAP:
			switch (_form) {
				case _Form::SMALL:
					_player->_animatedSprite.PlaySpriteAnimation("MapSmall", _player->_position);
					break;
				case _Form::BIG:
					_player->_animatedSprite.PlaySpriteAnimation("MapBig", _player->_position);
					break;
				case _Form::FIRE:
					_player->_animatedSprite.PlaySpriteAnimation("MapFire", _player->_position);
					break;
				case _Form::RACCOON:
					_player->_animatedSprite.PlaySpriteAnimation("MapRac", _player->_position);
					break;
			}
			break;
		case _State::IDLE:
			switch (_form) {
				case _Form::SMALL:
					_player->_animatedSprite.PlaySpriteAnimation("Idle", _player->_position);
					break;
				case _Form::BIG:
					_player->_animatedSprite.PlaySpriteAnimation("BigIdle", _player->_position);
					break;
				case _Form::FIRE:
					_player->_animatedSprite.PlaySpriteAnimation("FireIdle", _player->_position);
					break;
				case _Form::RACCOON:
					_player->_animatedSprite.PlaySpriteAnimation("RacIdle", _player->_position);
					break;
			}
			break;
		case _State::RUN:
			switch (_form) {
				case _Form::SMALL:

					break;
				case _Form::BIG:

					break;
				case _Form::FIRE:

					break;
				case _Form::RACCOON:

					break;
			}
			break;
		case _State::JUMP:
			switch (_form) {
				case _Form::SMALL:

					break;
				case _Form::BIG:

					break;
				case _Form::FIRE:

					break;
				case _Form::RACCOON:

					break;
			}
			break;
		case _State::FALL:
			switch (_form) {
				case _Form::SMALL:

					break;
				case _Form::BIG:

					break;
				case _Form::FIRE:

					break;
				case _Form::RACCOON:

					break;
			}
			break;
		case _State::CROUCH:
			/*
			if (_player->IsAttacking()) {
				_player->_animatedSprite.PlaySpriteAnimation("RacWag", _player->_position, D3DXVECTOR2(_player->_normal.x, 1.0f));
			}
			*/
			break;
		case _State::THROW:
			_player->_animatedSprite.PlaySpriteAnimation("FireThrow", _player->_position);
			break;
		case _State::WAG:

			break;
	}
}

void StateMachine::Release() {
	_player = nullptr;
}
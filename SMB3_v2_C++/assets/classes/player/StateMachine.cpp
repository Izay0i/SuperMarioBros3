#include "StateMachine.h"

void StateMachine::_HandleForms() {
	switch (_player->GetHealth()) {
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
			_player->GetAnimatedSprite().PlaySpriteAnimation("GrowUp", _player->GetPosition());
			break;
		case 2:
			_player->GetAnimatedSprite().PlaySpriteAnimation("SmokePuff", _player->GetPosition());
			break;
		case 3:
			_player->GetAnimatedSprite().PlaySpriteAnimation("SmokePuff", _player->GetPosition());
			break;
		case 4:
			_player->GetAnimatedSprite().PlaySpriteAnimation("SmokePuff", _player->GetPosition());
			break;
	}
}

void StateMachine::_OnExit() {
	switch (_player->GetHealth()) {
		case 1:
			_player->GetAnimatedSprite().PlaySpriteAnimation("ShrinkDown", _player->GetPosition());
			break;
		case 2:
			_player->GetAnimatedSprite().PlaySpriteAnimation("SmokePuff", _player->GetPosition());
			break;
		case 3:
			_player->GetAnimatedSprite().PlaySpriteAnimation("SmokePuff", _player->GetPosition());
			break;
		case 4:
			_player->GetAnimatedSprite().PlaySpriteAnimation("SmokePuff", _player->GetPosition());
			break;
	}
}

StateMachine::StateMachine(Player* player) {
	_scaleX = 1.0f;
	_alpha = 255;
	_currentHealth = player->GetHealth();
	_player = player;

	_HandleForms();
}

StateMachine::~StateMachine() {}

void StateMachine::HandleStates(int keyCode, bool isKeyDown) {	
	if (_currentHealth != _player->GetHealth()) {
		if (_currentHealth < _player->GetHealth()) {
			_OnEntry();
		}
		else if (_currentHealth > _player->GetHealth()) {
			_OnExit();
		}
		
		_currentHealth = _player->GetHealth();
		_HandleForms();
	}	

	GlobalUtil::KeyInput keyInput = static_cast<GlobalUtil::KeyInput>(keyCode);
	switch (_state) {
		case _State::MAP:
			
			break;
		case _State::IDLE:
			if (_player->GetVelocity().x != 0.0f && _player->IsOnGround()) {
				_state = _State::RUN;
			}
			else if (_player->GetVelocity().y < 0.0f) {
				_state = _State::JUMP;
			}
			else if (_player->GetVelocity().y > 0.0f && !_player->IsOnGround()) {
				_state = _State::FALL;
			}
			else if (_player->GetHeldEntity() == nullptr) {
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
			if (_player->GetVelocity().x == 0.0f) {
				_state = _State::IDLE;
			}
			else if (_player->GetVelocity().y < 0.0f) {
				_state = _State::JUMP;
			}
			else if (_player->GetVelocity().y > 0.0f && !_player->IsOnGround()) {
				_state = _State::FALL;
			}
			break;
		case _State::JUMP:
			if (_player->GetVelocity().y > 0.0f) {
				_state = _State::FALL;
			}
			/*
			else if (_player->IsAttacking()) {
				_state = _State::WAG;
			}
			*/
			break;
		case _State::FALL:
			if (_player->IsOnGround()) {
				_state = _State::IDLE;
			}
			/*
			else if (_player->IsAttacking()) {
				_state = _State::WAG;
			}
			*/
			break;
		case _State::CROUCH:
			if (_player->GetVelocity().x != 0.0f || 
				(!isKeyDown && keyInput == GlobalUtil::KeyInput::KEY_INPUT_DOWN)
				) 
			{
				_state = _State::IDLE;
			}
			break;
		case _State::THROW:
			if (_player->GetVelocity().x != 0.0f || 
				(!isKeyDown && keyInput == GlobalUtil::KeyInput::KEY_INPUT_B)) 
			{
				_state = _State::IDLE;
			}
			break;
		case _State::WAG:
			if (_player->GetVelocity().x != 0.0f || 
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
	if (_player->GetHealth() == 0) {
		_player->GetAnimatedSprite().PlaySpriteAnimation("Die", _player->GetPosition());
		return;
	}

	switch (_state) {
		case _State::MAP:
			switch (_form) {
				case _Form::SMALL:
					_player->GetAnimatedSprite().PlaySpriteAnimation("MapSmall", _player->GetPosition());
					break;
				case _Form::BIG:
					_player->GetAnimatedSprite().PlaySpriteAnimation("MapBig", _player->GetPosition());
					break;
				case _Form::FIRE:
					_player->GetAnimatedSprite().PlaySpriteAnimation("MapFire", _player->GetPosition());
					break;
				case _Form::RACCOON:
					_player->GetAnimatedSprite().PlaySpriteAnimation("MapRac", _player->GetPosition());
					break;
			}
			break;
		case _State::IDLE:
			switch (_form) {
				case _Form::SMALL:
					_player->GetAnimatedSprite().PlaySpriteAnimation("Idle", _player->GetPosition());
					break;
				case _Form::BIG:

					break;
				case _Form::FIRE:

					break;
				case _Form::RACCOON:

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
				_player->GetAnimatedSprite().PlaySpriteAnimation("RacWag", _player->GetPosition(), D3DXVECTOR2(_player->GetNormal().x, 1.0f));
			}
			*/
			break;
		case _State::THROW:
			_player->GetAnimatedSprite().PlaySpriteAnimation("FireThrow", _player->GetPosition());
			break;
		case _State::WAG:

			break;
	}
}

void StateMachine::Release() {
	_player = nullptr;
}
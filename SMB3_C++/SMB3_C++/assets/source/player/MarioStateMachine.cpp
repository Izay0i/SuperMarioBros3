#include "../../headers/player/MarioStateMachine.h"

MarioStateMachine::MarioStateMachine(Mario* mario) : StateMachine(mario) { 
	this->mario = mario;

	currentForm = MarioForm::BIG;
	currentState = MarioState::IDLE;
}

void MarioStateMachine::HandleStates(BYTE* states) {
	switch (currentState) {
		case MarioState::DIE:

			return;
		case MarioState::IDLE:
			if (mario->GetVelocity().x != 0.0f) {
				currentState = MarioState::RUN;
			}
			else if (mario->GetVelocity().y < 0.0f) {
				currentState = MarioState::JUMP;
			}
			else if (mario->GetVelocity().y > 0.0f && !mario->IsOnGround()) {
				currentState = MarioState::FALL;
			}
			else if (Device::IsKeyDown(DIK_S)) {
				currentState = MarioState::CROUCH;
			}
			break;
		case MarioState::RUN:
			if (mario->GetVelocity().x == 0.0f) {
				currentState = MarioState::IDLE;
			}
			else if (mario->GetVelocity().y < 0.0f) {
				currentState = MarioState::JUMP;
			}
			else if (mario->GetVelocity().y > 0.0f && !mario->IsOnGround()) {
				currentState = MarioState::FALL;
			}
			break;
		case MarioState::TURN:
			if (mario->GetVelocity().x == 0.0f) {
				currentState = MarioState::IDLE;
			}
			else if (mario->GetVelocity().x != 0.0f) {
				currentState = MarioState::RUN;
			}
			break;
		case MarioState::JUMP:
			if (mario->GetVelocity().y > 0.0f) {
				currentState = MarioState::FALL;
			}
			else if (mario->IsOnGround()) {
				currentState = MarioState::IDLE;
			}
			break;
		case MarioState::FALL:
			if (mario->IsOnGround()) {
				currentState = MarioState::IDLE;
			}
			break;
		case MarioState::CROUCH:
			if (!Device::IsKeyDown(DIK_S)) {
				currentState = MarioState::IDLE;
			}
			break;
		case MarioState::HOLD:

			break;
		case MarioState::KICK:

			break;
		//racoon
		case MarioState::SPIN:

			break;
		//fire
		case MarioState::SHOOT:

			break;
	}
}

void MarioStateMachine::Render() {
	switch (currentState) {
		case MarioState::DIE:
			mario->GetSprite().PlayAnimation("Die", mario->GetPosition(), mario->GetScale());
			return;
		case MarioState::IDLE:
			mario->GetSprite().PlayAnimation("BigIdle", mario->GetPosition(), mario->GetScale());
			break;
		case MarioState::RUN:
			mario->GetSprite().PlayAnimation("BigRun", mario->GetPosition(), mario->GetScale());
			break;
		case MarioState::TURN:
			mario->GetSprite().PlayAnimation("BigTurn", mario->GetPosition(), mario->GetScale());
			break;
		case MarioState::JUMP:
			mario->GetSprite().PlayAnimation("BigJump", mario->GetPosition(), mario->GetScale());
			break;
		case MarioState::FALL:
			mario->GetSprite().PlayAnimation("BigJump", mario->GetPosition(), mario->GetScale());
			break;
		case MarioState::CROUCH:
			mario->GetSprite().PlayAnimation("BigCrouch", mario->GetPosition(), mario->GetScale());
			break;
		case MarioState::HOLD:

			break;
		case MarioState::KICK:

			break;
		//racoon
		case MarioState::SPIN:

			break;
		//fire
		case MarioState::SHOOT:

			break;
	}
}
#include "../../headers/player/MarioStateMachine.h"

MarioStateMachine::MarioStateMachine(Mario* mario) : StateMachine(mario) { 
	this->mario = mario;

	currentForm = MarioForm::SMALL;
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
			else if (mario->GetVelocity().y > 0.0f) {
				currentState = MarioState::FALL;
			}
			break;
		case MarioState::RUN:
			if (mario->GetVelocity().x == 0.0f) {
				currentState = MarioState::IDLE;
			}
			else if (mario->GetVelocity().y < 0.0f) {
				currentState = MarioState::JUMP;
			}
			else if (mario->GetVelocity().y > 0.0f) {
				currentState = MarioState::FALL;
			}
			break;
		case MarioState::JUMP:
			if (mario->GetVelocity().y > 0.0f) {
				currentState = MarioState::FALL;
			}
			break;
		case MarioState::FALL:

			break;
		case MarioState::CROUCH:

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
			mario->GetSprite().PlayAnimation("Idle", mario->GetPosition(), mario->GetScale());
			break;
		case MarioState::RUN:
			mario->GetSprite().PlayAnimation("Run", mario->GetPosition(), mario->GetScale());
			break;
		case MarioState::JUMP:
			mario->GetSprite().PlayAnimation("Jump", mario->GetPosition(), mario->GetScale());
			break;
		case MarioState::FALL:
			mario->GetSprite().PlayAnimation("Jump", mario->GetPosition(), mario->GetScale());
			break;
		case MarioState::CROUCH:

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
#include "../../headers/player/MarioStateMachine.h"

MarioStateMachine::MarioStateMachine(Mario* mario) { 
	this->mario = mario;
}

void MarioStateMachine::HandleStates(BYTE* states) {
	switch (mario->GetCurrentHitPoints()) {
		case 1:
			currentForm = MarioForm::SMALL;
			break;
		case 2:
			currentForm = MarioForm::BIG;
			break;
		case 3:
			currentForm = MarioForm::FIRE;
			break;
		case 4:
			currentForm = MarioForm::RACOON;
			break;
	}
	
	switch (currentState) {
		case MarioState::IDLE:
			if (mario->GetCurrentHitPoints() == 0) {
				currentState = MarioState::DIE;
			}
			else if (mario->GetVelocity().x != 0.0f && mario->IsOnGround()) {
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
			else if (Device::IsKeyDown(DIK_K) && currentForm == MarioForm::FIRE) {
				currentState = MarioState::SHOOT;
			}
			else if (Device::IsKeyDown(DIK_K) && currentForm == MarioForm::RACOON) {
				currentState = MarioState::SPIN;
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
			if (!Device::IsKeyDown(DIK_S) || mario->GetVelocity().x != 0.0f) {
				currentState = MarioState::IDLE;
			}
			break;
		case MarioState::HOLD:

			break;
		case MarioState::KICK:

			break;		
		//fire
		case MarioState::SHOOT:
			if (!Device::IsKeyDown(DIK_K)) {
				currentState = MarioState::IDLE;
			}
			break;
			//racoon
		case MarioState::SPIN:
			if (!Device::IsKeyDown(DIK_K)) {
				currentState = MarioState::IDLE;
			}
			break;
	}
}

void MarioStateMachine::Render() {
	switch (currentState) {
		case MarioState::DIE:
			mario->GetSprite().PlayAnimation("Die", mario->GetPosition(), mario->GetScale());
			return;
		case MarioState::IDLE:
			switch (currentForm) {
				case MarioForm::SMALL:
					mario->GetSprite().PlayAnimation("Idle", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::BIG:
					mario->GetSprite().PlayAnimation("BigIdle", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::FIRE:
					mario->GetSprite().PlayAnimation("FireIdle", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::RACOON:
					mario->GetSprite().PlayAnimation("RacIdle", mario->GetPosition(), mario->GetScale());
					break;
			}
			break;
		case MarioState::RUN:
			switch (currentForm) {
				case MarioForm::SMALL:
					mario->GetSprite().PlayAnimation("Run", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::BIG:
					mario->GetSprite().PlayAnimation("BigRun", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::FIRE:
					mario->GetSprite().PlayAnimation("FireRun", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::RACOON:
					if (mario->GetAcceleration() >= mario->GetAccelThreshold() && mario->IsOnGround()) {
						mario->GetSprite().PlayAnimation("RacTakeOff", mario->GetPosition(), mario->GetScale());
					}
					else {
						mario->GetSprite().PlayAnimation("RacRun", mario->GetPosition(), mario->GetScale());
					}
					break;
			}
			break;
		case MarioState::TURN:
			/*switch (currentForm) {
				case MarioForm::SMALL:
					mario->GetSprite().PlayAnimation("Turn", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::BIG:
					mario->GetSprite().PlayAnimation("BigTurn", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::FIRE:
					mario->GetSprite().PlayAnimation("FireTurn", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::RACOON:
					mario->GetSprite().PlayAnimation("RacTurn", mario->GetPosition(), mario->GetScale());
					break;
			}*/
			break;
		case MarioState::JUMP:
			switch (currentForm) {
				case MarioForm::SMALL:
					mario->GetSprite().PlayAnimation("Jump", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::BIG:
					mario->GetSprite().PlayAnimation("BigJump", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::FIRE:
					mario->GetSprite().PlayAnimation("FireJump", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::RACOON:
					if (mario->GetAcceleration() >= mario->GetAccelThreshold() && mario->IsOnGround()) {
						mario->GetSprite().PlayAnimation("RacTakeOffJump", mario->GetPosition(), mario->GetScale());
					}
					else if (Device::IsKeyDown(DIK_SPACE) && mario->GetAcceleration() >= mario->GetAccelThreshold()) {
						mario->GetSprite().PlayAnimation("RacFlyBoost", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetAcceleration() >= mario->GetAccelThreshold()) {
						mario->GetSprite().PlayAnimation("RacFly", mario->GetPosition(), mario->GetScale());
					}					
					else {
						mario->GetSprite().PlayAnimation("RacJump", mario->GetPosition(), mario->GetScale());
					}
					break;
			}
			break;
		case MarioState::FALL:
			switch (currentForm) {
				case MarioForm::SMALL:
					mario->GetSprite().PlayAnimation("Jump", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::BIG:
					mario->GetSprite().PlayAnimation("BigJump", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::FIRE:
					mario->GetSprite().PlayAnimation("FireJump", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::RACOON:					
					if (Device::IsKeyDown(DIK_SPACE) && mario->GetAcceleration() < mario->GetAccelThreshold()) {
						mario->GetSprite().PlayAnimation("RacSlowFall", mario->GetPosition(), mario->GetScale());
					}
					else if (Device::IsKeyDown(DIK_SPACE) || Device::IsKeyDown(DIK_K) && mario->GetAcceleration() >= mario->GetAccelThreshold()) {
						mario->GetSprite().PlayAnimation("RacFlyBoost", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetAcceleration() >= mario->GetAccelThreshold()) {
						mario->GetSprite().PlayAnimation("RacFly", mario->GetPosition(), mario->GetScale());
					}
					else {
						mario->GetSprite().PlayAnimation("RacFall", mario->GetPosition(), mario->GetScale());
					}
					break;
			}
			break;
		case MarioState::CROUCH:
			switch (currentForm) {
				case MarioForm::SMALL:
					mario->GetSprite().PlayAnimation("Idle", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::BIG:
					mario->GetSprite().PlayAnimation("BigCrouch", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::FIRE:
					mario->GetSprite().PlayAnimation("FireCrouch", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::RACOON:
					mario->GetSprite().PlayAnimation("RacCrouch", mario->GetPosition(), mario->GetScale());
					break;
			}
			break;
		case MarioState::HOLD:

			break;
		case MarioState::KICK:

			break;
		//racoon
		case MarioState::SPIN:
			switch (currentForm) {
				case MarioForm::SMALL:
					mario->GetSprite().PlayAnimation("Idle", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::BIG:
					mario->GetSprite().PlayAnimation("BigIdle", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::FIRE:
					mario->GetSprite().PlayAnimation("FireIdle", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::RACOON:
					mario->GetSprite().PlayAnimation(
						"RacSpin",
						mario->GetPosition(),
						D3DXVECTOR2(mario->GetNormal().x, 1.0f)
					);
					break;
			}
			break;
		//fire
		case MarioState::SHOOT:
			switch (currentForm) {
				case MarioForm::SMALL:
					mario->GetSprite().PlayAnimation("Idle", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::BIG:
					mario->GetSprite().PlayAnimation("BigIdle", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::FIRE:
					mario->GetSprite().PlayAnimation("FireShoot", mario->GetPosition(), mario->GetScale());
					break;
				case MarioForm::RACOON:
					mario->GetSprite().PlayAnimation("RacIdle", mario->GetPosition(), mario->GetScale());
					break;
			}
			break;
	}
}
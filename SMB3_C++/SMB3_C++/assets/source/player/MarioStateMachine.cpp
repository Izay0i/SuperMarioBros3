#include "../../headers/player/MarioStateMachine.h"

MarioStateMachine::MarioStateMachine(Mario* mario) { 
	this->mario = mario;
}

//this doesnt seem to scale well with that many animations
//there can only be so many ifs you can cram into it before things start to mess up
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
		case MarioState::GUI:
			if (!mario->IsInMap()) {
				currentState = MarioState::IDLE;
			}
			break;
		case MarioState::IDLE:
			if (mario->IsInMap()) {
				currentState = MarioState::GUI;
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
			else if (Device::IsKeyDown(DIK_S) && !mario->GetHeldEntity()) {
				currentState = MarioState::CROUCH;
			}
			else if (Device::IsKeyDown(DIK_J) && currentForm == MarioForm::FIRE && !mario->GetHeldEntity()) {
				currentState = MarioState::SHOOT;
			}
			else if (mario->IsAttacking() && currentForm == MarioForm::RACOON && !mario->GetHeldEntity()) {
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
		case MarioState::JUMP:
			if (mario->GetVelocity().y > 0.0f) {
				currentState = MarioState::FALL;
			}
			else if (mario->IsOnGround()) {
				currentState = MarioState::IDLE;
			}
			else if (Device::IsKeyDown(DIK_J) && currentForm == MarioForm::RACOON && mario->GetVelocity().x == 0.0f) {
				currentState = MarioState::SPIN;
			}
			break;
		case MarioState::FALL:
			if (mario->IsOnGround()) {
				currentState = MarioState::IDLE;
			}
			else if (Device::IsKeyDown(DIK_J) && currentForm == MarioForm::RACOON && mario->GetVelocity().x == 0.0f) {
				currentState = MarioState::SPIN;
			}
			break;
		case MarioState::CROUCH:
			if (!Device::IsKeyDown(DIK_S) || mario->GetVelocity().x != 0.0f) {
				currentState = MarioState::IDLE;
			}
			break;
		//fire
		case MarioState::SHOOT:
			if (!Device::IsKeyDown(DIK_J) || mario->GetVelocity().x != 0.0f) {
				currentState = MarioState::IDLE;
			}
			break;
			//racoon
		case MarioState::SPIN:
			if (!Device::IsKeyDown(DIK_J) || mario->GetVelocity().x != 0.0f) {
				if (!mario->IsAttacking()) {
					currentState = MarioState::IDLE;
				}
			}
			break;
	}
}

void MarioStateMachine::Update(DWORD delta) {
	if (GetTickCount64() % 50 == 0) {
		scaleX = -scaleX;
	}
	
	if (mario->TriggeredStageEnd()) {
		currentState = MarioState::RUN;
	}
	
	if (mario->IsInvulnerable()) {
		std::random_device device;
		std::mt19937 rng(device());
		std::uniform_int_distribution<std::mt19937::result_type> dist(63, 128);

		alpha = dist(rng);
	}
	else {
		alpha = 255;
	}
}

void MarioStateMachine::Render() {
	if (mario->GetCurrentHitPoints() == 0) {
		mario->GetSprite().PlayAnimation("Die", mario->GetPosition(), mario->GetScale());
		return;
	}

	switch (currentState) {
		case MarioState::GUI:
			switch (currentForm) {
				case MarioForm::SMALL:
					mario->GetSprite().PlayAnimation("GUI", mario->GetPosition(), D3DXVECTOR2(scaleX, 1.0f));
					break;
				case MarioForm::BIG:
					mario->GetSprite().PlayAnimation("BigGUI", D3DXVECTOR3(mario->GetPosition().x, mario->GetPosition().y - 8, 0), D3DXVECTOR2(scaleX, 1.0f));
					break;
				case MarioForm::FIRE:
					mario->GetSprite().PlayAnimation("FireGUI", D3DXVECTOR3(mario->GetPosition().x, mario->GetPosition().y - 8, 0), D3DXVECTOR2(scaleX, 1.0f));
					break;
				case MarioForm::RACOON:
					mario->GetSprite().PlayAnimation("RacGUI", D3DXVECTOR3(mario->GetPosition().x, mario->GetPosition().y - 8, 0), D3DXVECTOR2(scaleX, 1.0f));
					break;
			}
			break;
		case MarioState::IDLE:
			switch (currentForm) {
				case MarioForm::SMALL:
					if (mario->IsInPipe()) {
						mario->GetSprite().PlayAnimation("Front", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->IsNextToShell()) {
						mario->GetSprite().PlayAnimation("Kick", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("HoldIdle", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else {
						mario->GetSprite().PlayAnimation("Idle", mario->GetPosition(), mario->GetScale(), alpha);
					}
					break;
				case MarioForm::BIG:
					if (mario->IsInPipe()) {
						mario->GetSprite().PlayAnimation("BigFront", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->IsNextToShell()) {
						mario->GetSprite().PlayAnimation("BigKick", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("BigHoldIdle", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else {
						mario->GetSprite().PlayAnimation("BigIdle", mario->GetPosition(), mario->GetScale(), alpha);
					}
					break;
				case MarioForm::FIRE:
					if (mario->IsInPipe()) {
						mario->GetSprite().PlayAnimation("FireFront", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->IsNextToShell()) {
						mario->GetSprite().PlayAnimation("FireKick", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("FireHoldIdle", mario->GetPosition(), mario->GetScale(), alpha);
					}	
					else {
						mario->GetSprite().PlayAnimation("FireIdle", mario->GetPosition(), mario->GetScale(), alpha);
					}
					break;
				case MarioForm::RACOON:
					if (mario->IsInPipe()) {
						mario->GetSprite().PlayAnimation("RacFront", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->IsNextToShell()) {
						mario->GetSprite().PlayAnimation("RacKick", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("RacHoldIdle", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else {
						mario->GetSprite().PlayAnimation("RacIdle", mario->GetPosition(), mario->GetScale(), alpha);
					}
					break;
			}
			break;
		case MarioState::RUN:
			switch (currentForm) {
				case MarioForm::SMALL:
					if (mario->IsInPipe()) {
						mario->GetSprite().PlayAnimation("Front", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetAcceleration() >= mario->GetAccelThreshold() && mario->IsOnGround() && !mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("TakeOff", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else if (mario->IsNextToShell()) {
						mario->GetSprite().PlayAnimation("Kick", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("HoldRun", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else {
						if (mario->GetAcceleration() < 0.5f && (Device::IsKeyDown(DIK_A) || Device::IsKeyDown(DIK_D))) {
							mario->GetSprite().PlayAnimation("Skid", mario->GetPosition(), mario->GetScale(), alpha);
						}
						else {
							mario->GetSprite().PlayAnimation("Run", mario->GetPosition(), mario->GetScale(), alpha);
						}
					}
					break;
				case MarioForm::BIG:
					if (mario->IsInPipe()) {
						mario->GetSprite().PlayAnimation("BigFront", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetAcceleration() >= mario->GetAccelThreshold() && mario->IsOnGround() && !mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("BigTakeOff", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else if (mario->IsNextToShell()) {
						mario->GetSprite().PlayAnimation("BigKick", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("BigHoldRun", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else {
						if (mario->GetAcceleration() < 0.5f && (Device::IsKeyDown(DIK_A) || Device::IsKeyDown(DIK_D))) {
							mario->GetSprite().PlayAnimation("BigSkid", mario->GetPosition(), mario->GetScale(), alpha);
						}
						else {
							mario->GetSprite().PlayAnimation("BigRun", mario->GetPosition(), mario->GetScale(), alpha);
						}
					}
					break;
				case MarioForm::FIRE:
					if (mario->IsInPipe()) {
						mario->GetSprite().PlayAnimation("FireFront", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetAcceleration() >= mario->GetAccelThreshold() && mario->IsOnGround() && !mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("FireTakeOff", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else if (mario->IsNextToShell()) {
						mario->GetSprite().PlayAnimation("FireKick", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("FireHoldRun", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else {
						if (mario->GetAcceleration() < 0.5f && (Device::IsKeyDown(DIK_A) || Device::IsKeyDown(DIK_D))) {
							mario->GetSprite().PlayAnimation("FireSkid", mario->GetPosition(), mario->GetScale(), alpha);
						}
						else {
							mario->GetSprite().PlayAnimation("FireRun", mario->GetPosition(), mario->GetScale(), alpha);
						}
					}
					break;
				case MarioForm::RACOON:
					if (mario->IsInPipe()) {
						mario->GetSprite().PlayAnimation("RacFront", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetAcceleration() >= mario->GetAccelThreshold() && mario->IsOnGround() && !mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("RacTakeOff", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else if (mario->IsNextToShell()) {
						mario->GetSprite().PlayAnimation("RacKick", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("RacHoldRun", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else {
						if (mario->GetAcceleration() < 0.5f && (Device::IsKeyDown(DIK_A) || Device::IsKeyDown(DIK_D))) {
							mario->GetSprite().PlayAnimation("RacSkid", D3DXVECTOR3(mario->GetPosition().x, mario->GetPosition().y - 2, 0), mario->GetScale());
						}
						else {
							mario->GetSprite().PlayAnimation("RacRun", mario->GetPosition(), mario->GetScale(), alpha);
						}
					}
					break;
			}
			break;
		case MarioState::JUMP:
			switch (currentForm) {
				case MarioForm::SMALL:
					if (mario->IsInPipe()) {
						mario->GetSprite().PlayAnimation("Front", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetAcceleration() >= mario->GetAccelThreshold() && !mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("Fly", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else if (mario->IsNextToShell()) {
						mario->GetSprite().PlayAnimation("Kick", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("HoldJump", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else {
						mario->GetSprite().PlayAnimation("Jump", mario->GetPosition(), mario->GetScale(), alpha);
					}
					break;
				case MarioForm::BIG:
					if (mario->IsInPipe()) {
						mario->GetSprite().PlayAnimation("BigFront", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetAcceleration() >= mario->GetAccelThreshold() && mario->IsOnGround() && !mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("BigTakeOffJump", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else if (mario->GetAcceleration() >= mario->GetAccelThreshold() && !mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("BigFly", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else if (mario->IsNextToShell()) {
						mario->GetSprite().PlayAnimation("BigKick", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("BigHoldJump", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else {
						mario->GetSprite().PlayAnimation("BigJump", mario->GetPosition(), mario->GetScale(), alpha);
					}
					break;
				case MarioForm::FIRE:
					if (mario->IsInPipe()) {
						mario->GetSprite().PlayAnimation("FireFront", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetAcceleration() >= mario->GetAccelThreshold() && mario->IsOnGround() && !mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("FireTakeOffJump", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else if (mario->GetAcceleration() >= mario->GetAccelThreshold() && !mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("FireFly", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else if (mario->IsNextToShell()) {
						mario->GetSprite().PlayAnimation("FireKick", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("FireHoldJump", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else {
						mario->GetSprite().PlayAnimation("FireJump", mario->GetPosition(), mario->GetScale(), alpha);
					}
					break;
				case MarioForm::RACOON:
					if (mario->IsInPipe()) {
						mario->GetSprite().PlayAnimation("RacFront", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetAcceleration() >= mario->GetAccelThreshold() && mario->IsOnGround() && !mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("RacTakeOffJump", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else if (mario->IsFlying() && !mario->GetHeldEntity()) {
						if (Device::IsKeyDown(DIK_K)) {
							mario->GetSprite().PlayAnimation("RacFlyBoost", mario->GetPosition(), mario->GetScale(), alpha);
						}
						else {
							mario->GetSprite().PlayAnimation("RacFly", mario->GetPosition(), mario->GetScale(), alpha);
						}
					}
					else if (mario->IsNextToShell()) {
						mario->GetSprite().PlayAnimation("RacKick", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("RacHoldJump", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else {
						mario->GetSprite().PlayAnimation("RacJump", mario->GetPosition(), mario->GetScale(), alpha);
					}
					break;
			}
			break;
		case MarioState::FALL:
			switch (currentForm) {
				case MarioForm::SMALL:
					if (mario->IsInPipe()) {
						mario->GetSprite().PlayAnimation("Front", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetAcceleration() >= mario->GetAccelThreshold() && !mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("Fly", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else if (mario->IsNextToShell()) {
						mario->GetSprite().PlayAnimation("Kick", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("HoldJump", mario->GetPosition(), mario->GetScale());
					}
					else {
						mario->GetSprite().PlayAnimation("Jump", mario->GetPosition(), mario->GetScale(), alpha);
					}
					break;
				case MarioForm::BIG:
					if (mario->IsInPipe()) {
						mario->GetSprite().PlayAnimation("BigFront", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetAcceleration() >= mario->GetAccelThreshold() && !mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("BigFly", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else if (mario->IsNextToShell()) {
						mario->GetSprite().PlayAnimation("BigKick", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("BigHoldJump", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else {
						mario->GetSprite().PlayAnimation("BigFall", mario->GetPosition(), mario->GetScale(), alpha);
					}
					break;
				case MarioForm::FIRE:
					if (mario->IsInPipe()) {
						mario->GetSprite().PlayAnimation("FireFront", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetAcceleration() >= mario->GetAccelThreshold() && !mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("FireFly", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else if (mario->IsNextToShell()) {
						mario->GetSprite().PlayAnimation("FireKick", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("FireHoldJump", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else {
						mario->GetSprite().PlayAnimation("FireFall", mario->GetPosition(), mario->GetScale(), alpha);
					}
					break;
				case MarioForm::RACOON:
					if (mario->IsInPipe()) {
						mario->GetSprite().PlayAnimation("RacFront", mario->GetPosition(), mario->GetScale());
					}
					else if (Device::IsKeyDown(DIK_K) && !mario->IsFlying() && !mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("RacSlowFall", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else if (mario->IsFlying() && !mario->GetHeldEntity()) {
						if (Device::IsKeyDown(DIK_K)) {
							mario->GetSprite().PlayAnimation("RacFlyBoost", mario->GetPosition(), mario->GetScale(), alpha);
						}
						else {
							mario->GetSprite().PlayAnimation("RacFly", mario->GetPosition(), mario->GetScale(), alpha);
						}
					}
					else if (mario->IsNextToShell()) {
						mario->GetSprite().PlayAnimation("RacKick", mario->GetPosition(), mario->GetScale());
					}
					else if (mario->GetHeldEntity()) {
						mario->GetSprite().PlayAnimation("RacHoldJump", mario->GetPosition(), mario->GetScale(), alpha);
					}
					else {
						mario->GetSprite().PlayAnimation("RacFall", mario->GetPosition(), mario->GetScale(), alpha);
					}
					break;
			}
			break;
		case MarioState::CROUCH:
			switch (currentForm) {
				case MarioForm::SMALL:
					if (mario->IsInPipe()) {
						mario->GetSprite().PlayAnimation("Front", mario->GetPosition(), mario->GetScale());
					}
					else {
						mario->GetSprite().PlayAnimation("Idle", mario->GetPosition(), mario->GetScale(), alpha);
					}
					break;
				case MarioForm::BIG:
					if (mario->IsInPipe()) {
						mario->GetSprite().PlayAnimation("BigFront", mario->GetPosition(), mario->GetScale());
					}
					else {
						mario->GetSprite().PlayAnimation("BigCrouch", D3DXVECTOR3(mario->GetPosition().x, mario->GetPosition().y - 12, 0), mario->GetScale(), alpha);
					}
					break;
				case MarioForm::FIRE:
					if (mario->IsInPipe()) {
						mario->GetSprite().PlayAnimation("FireFront", mario->GetPosition(), mario->GetScale());
					}
					else {
						mario->GetSprite().PlayAnimation("FireCrouch", D3DXVECTOR3(mario->GetPosition().x, mario->GetPosition().y - 12, 0), mario->GetScale(), alpha);
					}
					break;
				case MarioForm::RACOON:
					if (mario->IsInPipe()) {
						mario->GetSprite().PlayAnimation("RacFront", mario->GetPosition(), mario->GetScale());
					}
					else {
						mario->GetSprite().PlayAnimation("RacCrouch", D3DXVECTOR3(mario->GetPosition().x, mario->GetPosition().y - 12, 0), mario->GetScale(), alpha);
					}
					break;
			}
			break;
			//racoon
		case MarioState::SPIN:
			if (mario->IsInPipe()) {
				mario->GetSprite().PlayAnimation("RacFront", mario->GetPosition(), mario->GetScale());
			}
			else if (mario->IsAttacking()) {
				mario->GetSprite().PlayAnimation(
					"RacSpin",
					mario->GetPosition(),
					D3DXVECTOR2(mario->GetNormal().x, 1.0f),
					alpha
				);

				if (mario->GetTouchedEntity()) {
					mario->GetSprite().PlayAnimation("Spark", D3DXVECTOR3(mario->GetPosition().x + (16 * mario->GetNormal().x), mario->GetPosition().y + 10, 0), mario->GetScale());
				}
			}
			else {
				mario->GetSprite().PlayAnimation("RacIdle", mario->GetPosition(), mario->GetScale(), alpha);
			}
			break;
			//fire
		case MarioState::SHOOT:
			if (mario->IsInPipe()) {
				mario->GetSprite().PlayAnimation("FireFront", mario->GetPosition(), mario->GetScale());
			}
			else {
				mario->GetSprite().PlayAnimation("FireShoot", mario->GetPosition(), mario->GetScale(), alpha);
			}
			break;
		}
}

void MarioStateMachine::Release() {
	if (mario) {
		delete mario;
		mario = nullptr;
	}
}
﻿#pragma once

#include "Mario.h"
#include "../StateMachine.h"

class Mario;

class MarioStateMachine : public StateMachine {
private:
	enum class MarioForm {
		SMALL,
		BIG,
		FIRE,
		RACOON,

		//Out of your friends
		//Which one are you?
		TruckFreak,
		CrazyAss,
		друг,
		TheFighter,

		Trickster,
		Swordmaster,
		Gunslinger,
		Royalguard
	};

	//Some states only available in certain forms
	enum class MarioState {
		IDLE,
		RUN,
		JUMP,
		FALL,
		//BIG, FIRE, RACOON
		CROUCH,
		//FIRE
		SHOOT,
		//RACOON
		SPIN,
	};

	Mario* mario;

	MarioForm currentForm;
	MarioState currentState;

public:
	MarioStateMachine(Mario*);

	void HandleStates(BYTE*) override;
	void Render() override;
};
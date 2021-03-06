﻿#pragma once

#include <random>

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
		GUI, //why did I call it GUI? idk but the animations are *GUI so changing them would be a waste of time anyway
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

	float scaleX = 1.0f; //no idea why the animation wont play tbh
	unsigned int alpha = 255;

	Mario* mario;

	MarioForm currentForm;
	MarioState currentState;

public:
	MarioStateMachine(Mario*);

	void HandleStates(BYTE*) override;
	void Update(DWORD) override;
	void Render() override;

	void Release();
};
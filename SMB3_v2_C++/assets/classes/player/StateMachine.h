#pragma once

#include "Player.h"

#include <random>

class Player;

class StateMachine {
private:
	enum class _Form {
		SMALL,
		BIG,
		FIRE,
		RACCOON,

		Trickster,
		Swordmaster,
		Gunslinger,
		RoyalGuard,
		I_AM_THE_STORM_THAT_IS_APPROACHING
	};

	enum class _State {
		MAP,
		IDLE,
		RUN,
		JUMP,
		FALL,
		CROUCH,
		THROW,
		WAG
	};

	float _scaleX;
	unsigned int _alpha;
	int _currentHealth;

	_Form _form;
	_State _state;

	Player* _player;

	void _HandleForms();
	void _OnEntry();
	void _OnExit();

public:
	StateMachine(Player*);
	~StateMachine();

	void HandleStates(int, bool);
	void Update(DWORD);
	void Render();

	void Release();
};

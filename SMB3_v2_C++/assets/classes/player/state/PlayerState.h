#pragma once

#include "../Player.h"

class Player;
class PlayerState {
protected:
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

	static Player* _player;

	int _currentHealth;

	_Form _form;

	void _OnEntry();
	void _OnExit();

public:
	virtual ~PlayerState();
	
	virtual PlayerState* HandleStates() = 0;
	virtual void Update(DWORD);
	virtual void Render();

	virtual void Release();
};
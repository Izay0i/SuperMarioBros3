#pragma once

#include "../Player.h"

class Player;
class PlayerState {
protected:
	enum class _Form {
		SMALL = 1,
		BIG = 2,
		FIRE = 3,
		RACCOON = 4,

		Trickster,
		Swordmaster,
		Gunslinger,
		RoyalGuard,
		I_AM_THE_STORM_THAT_IS_APPROACHING
	};

	static Player* _player;

	unsigned int _alpha;
	int _currentHealth;

	_Form _form;

	void _OnTransform();

public:
	PlayerState(Player*);
	virtual ~PlayerState();
	
	virtual PlayerState* HandleStates() = 0;
	virtual void Update(DWORD);
	virtual void Render();

	virtual void Release();
};
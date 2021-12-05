#pragma once

#include "../../Entity.h"
#include "../FortressBoss.h"

class FortressBoss;
class BossState {
protected:
	static FortressBoss* _fortressBoss;

public:
	BossState(FortressBoss*);
	virtual ~BossState();

	virtual BossState* HandleStates() = 0;
	virtual void Update(DWORD);
	virtual void Render();

	virtual void Release();
};
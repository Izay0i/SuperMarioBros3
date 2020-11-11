#pragma once

#include "Device.h"
#include "Entity.h"

class StateMachine {
protected:
	Entity* currentEntity;

	virtual void HandleStates(BYTE*) = 0;
	virtual void Render() = 0;

	StateMachine(Entity* en) { currentEntity = en; }
	virtual ~StateMachine() { if (currentEntity) { delete currentEntity; } }
};
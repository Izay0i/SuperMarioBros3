#pragma once

#include "Device.h"

class StateMachine {
protected:
	virtual void HandleStates(BYTE*) = 0;
	virtual void Render() = 0;

	StateMachine() {}
	virtual ~StateMachine() { }
};
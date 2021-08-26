#pragma once

class RunState : public PlayerState {
public:
	RunState(Player*);
	
	PlayerState* HandleStates() override;
	void Render() override;
};
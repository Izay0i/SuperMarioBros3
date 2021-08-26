#pragma once

class JumpState : public PlayerState {
public:
	JumpState(Player*);

	PlayerState* HandleStates() override;
	void Render() override;
};
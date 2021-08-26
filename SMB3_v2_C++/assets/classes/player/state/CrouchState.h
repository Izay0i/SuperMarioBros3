#pragma once

class CrouchState : public PlayerState {
public:
	CrouchState(Player*);

	PlayerState* HandleStates() override;
	void Render() override;
};
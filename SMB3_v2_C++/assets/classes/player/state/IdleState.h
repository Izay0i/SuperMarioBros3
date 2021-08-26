#pragma once

class IdleState : public PlayerState {
public:
	IdleState(Player*);

	PlayerState* HandleStates() override;
	void Render() override;
};
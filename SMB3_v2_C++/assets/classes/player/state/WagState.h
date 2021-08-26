#pragma once

class WagState : public PlayerState {
public:
	WagState(Player*);

	PlayerState* HandleStates() override;
	void Render() override;
};
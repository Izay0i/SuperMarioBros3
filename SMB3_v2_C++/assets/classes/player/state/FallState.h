#pragma once

class FallState : public PlayerState {
public:
	FallState(Player*);

	PlayerState* HandleStates() override;
	void Render() override;
};
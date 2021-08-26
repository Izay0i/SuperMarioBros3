#pragma once

class ThrowState : public PlayerState{
public:
	ThrowState(Player*);

	PlayerState* HandleStates() override;
	void Render() override;
};
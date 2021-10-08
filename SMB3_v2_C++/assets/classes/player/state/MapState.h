#pragma once

class MapState : public PlayerState {
public:
	MapState(Player*);

	PlayerState* HandleStates() override;
	void Render() override;
};
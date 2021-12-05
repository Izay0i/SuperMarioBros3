#pragma once

class BossIdleState : public BossState {
public:
	BossIdleState(FortressBoss*);

	BossState* HandleStates() override;
	void Render() override;
};
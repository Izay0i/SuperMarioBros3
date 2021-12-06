#pragma once

class BossIntroState : public BossState {
public:
	BossIntroState(FortressBoss*);

	BossState* HandleStates() override;
	void Render() override;
};
#pragma once

class BossRunState : public BossState {
public:
	BossRunState(FortressBoss*);

	BossState* HandleStates() override;

	void Update(DWORD) override;
	void Render() override;
};
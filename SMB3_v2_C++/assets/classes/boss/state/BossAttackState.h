#pragma once

class BossAttackState : public BossState {
public:
	BossAttackState(FortressBoss*);

	BossState* HandleStates() override;

	void Update(DWORD);
	void Render();
};
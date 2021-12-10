#pragma once

class BossAttackState : public BossState {
private:
	DWORD _jumpStart;
	DWORD _jumpTime;

public:
	bool IsJumping() const;
	void StartJumpTimer();

	BossAttackState(FortressBoss*);

	BossState* HandleStates() override;

	void Update(DWORD);
	void Render();
};
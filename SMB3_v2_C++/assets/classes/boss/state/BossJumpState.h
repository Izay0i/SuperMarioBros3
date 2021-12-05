#pragma once

class BossJumpState : public BossState {
public:
	BossJumpState(FortressBoss*);

	BossState* HandleStates() override;
	void Update(DWORD);
	void Render() override;
};
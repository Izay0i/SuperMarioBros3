#pragma once

class BossHurtState : public BossState {
public:
	BossHurtState(FortressBoss*);

	BossState* HandleStates() override;
	
	void Update(DWORD);
	void Render() override;
};
#pragma once

class BossHurtState : public BossState {
private:
	float _scaleX;

public:
	BossHurtState(FortressBoss*);

	BossState* HandleStates() override;
	
	void Update(DWORD);
	void Render() override;
};
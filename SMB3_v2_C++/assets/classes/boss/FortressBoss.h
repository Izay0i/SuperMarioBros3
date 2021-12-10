#pragma once

#include "state/BossState.h"

class Orb;
class OrbEffect;
class BossState;
class FortressBoss : public Entity {
private:
	friend class BossState;
	friend class BossIntroState;
	friend class BossIdleState;
	friend class BossRunState;
	friend class BossAttackState;
	friend class BossHurtState;

	static Texture* _bossTexture;

	BossState* _bossState;

	DWORD _introStart;
	DWORD _introTime;

	DWORD _attackStart;
	DWORD _attackTime;

	DWORD _coolDownStart;
	DWORD _coolDownTime;

	DWORD _invulnerableStart;
	DWORD _invulnerableTime;

	void _ParseSprites(std::string) override;

public:
	bool commenceBattle;

	FortressBoss();
	~FortressBoss();

	bool IsInIntro() const;
	void StartIntroTimer();

	bool IsAttacking() const;
	void StartAttackTimer();

	bool IsOnCoolDown() const;
	void StartCoolDownTimer();

	bool IsInvulnerable() const;
	void StartInvulnerableTimer();

	RECTF GetBoundingBox(int = 0) const override;

	Orb* SpawnOrb();
	OrbEffect* SpawnOrbEffect();

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void TakeDamage() override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
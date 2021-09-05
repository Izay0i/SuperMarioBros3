#pragma once

class BoomerBro : public Entity {
private:
	enum class _State {
		WALK,
		DIE
	};

	static LPDIRECT3DTEXTURE9 _boomerTexture;

	_State _state;

	D3DXVECTOR2 _originalPos;

	DWORD _attackStart;
	DWORD _attackTime;

	void _ParseSprites(std::string) override;

public:
	BoomerBro();
	~BoomerBro();

	RECTF GetBoundingBox(int = 0) const override;

	void SetPosition(D3DXVECTOR2) override;

	bool IsAttacking() const;
	void StartAttackTimer();

	//Boomerang* SpawnBoomerang();

	void ParseData(std::string, const LPDIRECT3DTEXTURE9&, std::vector<std::string> = std::vector<std::string>()) override;

	void TakeDamage() override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
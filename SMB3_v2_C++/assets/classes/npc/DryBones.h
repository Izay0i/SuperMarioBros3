#pragma once

class DryBones : public Entity {
private:
	enum class _State {
		WALK = 3,
		LIMBO = 2
	};

	static Texture* _dryTexture;

	_State _state;

	DWORD _limboStart;
	DWORD _limboTime;

	void _ParseSprites(std::string) override;

public:
	DryBones();
	~DryBones();

	bool IsInLimbo() const;
	void StartLimboTimer();

	RECTF GetBoundingBox(int = 0) const override;

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void TakeDamage() override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
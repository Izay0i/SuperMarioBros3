#pragma once

class Koopa : public Entity {
protected:
	enum class _State {
		FLY = 4,
		WALK = 3,
		RETRACT = 2,
		SPIN = 1,
		DIE = 0
	};

	static Texture* _koopaTexture;

	_State _state;

	DWORD _retractStart;
	DWORD _retractTime;

	void _ParseSprites(std::string) override;

public:
	Koopa();
	virtual ~Koopa();

	bool IsRetracting() const;
	void StartRetractTimer();

	RECTF GetBoundingBox(int = 0) const override;

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void TakeDamage() override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
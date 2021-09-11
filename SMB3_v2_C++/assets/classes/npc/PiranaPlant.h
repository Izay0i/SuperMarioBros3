#pragma once

class PiranaPlant : public Entity {
protected:
	enum class _State {
		BITE = 1,
		DIE = 0
	};

	const float _MAX_Y_OFFSET = 38.0f;

	static Texture* _piranaTexture;

	_State _state;

	bool _isPlayerInRange;

	RECTF _boundary;

	D3DXVECTOR2 _originalPos;

	DWORD _coolDownStart;
	DWORD _coolDownTime;

	void _ParseSprites(std::string) override;

public:
	PiranaPlant();
	virtual ~PiranaPlant();

	bool IsOnCoolDown() const;
	void StartCoolDownTimer();

	RECTF GetBoundingBox(int = 0) const override;

	void SetPosition(D3DXVECTOR2) override;

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void TakeDamage() override;

	void ComparePlayerPosToSelf(D3DXVECTOR2);

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
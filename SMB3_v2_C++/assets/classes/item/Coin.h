#pragma once

class Coin : public Entity {
private:
	enum class _State {
		ROTATE = 1,
		PUSHEDFROMBLOCK = 2,
		BRICK = 3
	};

	static Texture* _coinTexture;

	_State _state;

	DWORD _popUpStart;
	DWORD _popUpTime;

	void _ParseSprites(std::string) override;

public:
	Coin();
	~Coin();

	bool IsPoppedUp() const;
	void StartPopUpTimer();

	RECTF GetBoundingBox(int = 0) const override;

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void TakeDamage() override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
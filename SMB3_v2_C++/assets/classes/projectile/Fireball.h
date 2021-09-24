#pragma once

class Fireball : public Entity {
private:
	enum class _State {
		BOUNCE = 1,
		EXPLODE = 0
	};
	
	static Texture* _fireballTexture;

	float _travelSpeed;

	_State _state;

	DWORD _aliveStart;
	DWORD _aliveTime;

	void _ParseSprites(std::string) override;

public:
	Fireball();
	~Fireball();

	bool IsAlive() const;
	void StartAliveTimer();

	RECTF GetBoundingBox(int = 0) const override;

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
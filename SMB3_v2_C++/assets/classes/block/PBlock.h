#pragma once

class PBlock : public Entity {
private:
	enum class _State {
		ACTIVE = 1,
		INACTIVE = 2
	};

	//Yeah calling this PBlock was a bad idea
	static Texture* _pBlockTexture;

	_State _state;

	DWORD _activateStart;
	DWORD _activateTime;

	void _ParseSprites(std::string) override;

public:
	PBlock();
	~PBlock();
	
	bool IsActivated() const;
	void StartActivationTimer();

	RECTF GetBoundingBox(int = 0) const override;

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void TakeDamage() override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
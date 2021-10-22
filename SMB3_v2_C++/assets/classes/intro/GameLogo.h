#pragma once

class GameLogo : public Entity {
private:
	static Texture* _logoTexture;

	DWORD _fallDownStart;
	DWORD _fallDownTime;

	void _ParseSprites(std::string) override;

public:
	GameLogo();
	~GameLogo();
	
	bool IsFallingDown() const;
	void StartFallDownTimer();

	RECTF GetBoundingBox(int = 0) const override;

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
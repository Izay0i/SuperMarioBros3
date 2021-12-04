#pragma once

class Podoboo : public Entity {
private:
	static Texture* _podTexture;

	const float _OFFSET = 16.0f;
	const float _MAX_HEIGHT = 112.0f;

	D3DXVECTOR2 _originalPos;

	DWORD _inactiveStart;
	DWORD _inactiveTime;

	void _ParseSprites(std::string) override;

public:
	Podoboo();
	~Podoboo();

	bool IsInactive() const;
	void StartInactiveTimer();

	RECTF GetBoundingBox(int = 0) const override;

	void SetPosition(D3DXVECTOR2) override;

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
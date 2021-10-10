#pragma once

class Curtain : public Entity {
private:
	static Texture* _curtainTexture;

	DWORD _curlUpStart;
	DWORD _curlUpTime;

	void _ParseSprites(std::string) override;

public:
	Curtain();
	~Curtain();

	bool IsCurlingUp() const;
	void StartCurlUpTimer();

	RECTF GetBoundingBox(int = 0) const override;

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
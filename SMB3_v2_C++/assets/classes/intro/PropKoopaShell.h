#pragma once

class PropKoopaShell : public Entity {
private:
	static Texture* _shellTexture;

	void _ParseSprites(std::string) override;

public:
	std::string animationName;

	PropKoopaShell();
	~PropKoopaShell();

	RECTF GetBoundingBox(int = 0) const override;

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
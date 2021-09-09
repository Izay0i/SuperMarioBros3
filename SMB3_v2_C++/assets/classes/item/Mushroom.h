#pragma once

class Mushroom : public Entity {
private:
	static LPDIRECT3DTEXTURE9 _shroomTexture;

	DWORD _emergeStart;
	DWORD _emergeTime;

	void _ParseSprites(std::string) override;

public:
	Mushroom();
	~Mushroom();

	bool IsEmerging() const;
	void StartEmergeTimer();

	RECTF GetBoundingBox(int = 0) const override;

	void ParseData(std::string, const LPDIRECT3DTEXTURE9&, std::vector<std::string> = std::vector<std::string>()) override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
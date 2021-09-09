#pragma once

class Boomerang : public Entity {
private:
	static LPDIRECT3DTEXTURE9 _boomerangTexture;

	D3DXVECTOR2 _originalPos;

	void _ParseSprites(std::string);

public:
	Boomerang();
	~Boomerang();

	void SetPosition(D3DXVECTOR2) override;

	void ParseData(std::string, const LPDIRECT3DTEXTURE9&, std::vector<std::string> = std::vector<std::string>()) override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
#pragma once

class Palkia : public Entity {
private:
	static Texture* _palkiaTexture;

	DWORD _cryStart;
	DWORD _cryTime;

	void _ParseSprites(std::string) override;

public:
	Palkia();
	~Palkia();

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
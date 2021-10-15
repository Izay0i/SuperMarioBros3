#pragma once

class Portal : public Entity {
private:
	static Texture* _portalTexture;

	D3DXVECTOR2 _destination;

	void _ParseSprites(std::string) override;

public:
	Portal();
	~Portal();

	unsigned int GetSceneID() const;
	D3DXVECTOR2 GetDestination() const;

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;
	
	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
#pragma once

class Door : public Entity {
private:
	static Texture* _doorTexture;

	D3DXVECTOR2 _destination;

	void _ParseSprites(std::string) override;

public:
	Door();
	~Door();

	D3DXVECTOR2 GetDestination() const;

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void TakeDamage() override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
#pragma once

class Parakoopa : public Koopa {
private:
	D3DXVECTOR2 _originalPos;

public:
	Parakoopa();
	~Parakoopa();

	void SetPosition(D3DXVECTOR2) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
};
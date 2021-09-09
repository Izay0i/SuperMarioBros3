#pragma once

class Parakoopa : public Koopa {
public:
	Parakoopa();
	~Parakoopa();

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
};
#pragma once

class Paragoomba : public Goomba {
private:
	unsigned int _jumpCount;

	DWORD _walkStart;
	DWORD _walkTime;

	void _HandleJumping();

public:
	Paragoomba();
	~Paragoomba();

	bool IsWalking() const;
	void StartWalkTimer();

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
};
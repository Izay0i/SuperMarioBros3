#pragma once

#include <fstream>

#include "../Entity.h"
#include "../TileList.h"
#include "Goomba.h"

class Entity;

class Paragoomba : public Goomba {
private:
	bool isOnGround;

	int jumpCount;

	float runSpeed = 0.03f;
	float jumpSpeed = 0.006f;
	float gravity = 0.001f;

	void HandleJumping();
	void HandleStates() override;

public:
	Paragoomba();

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
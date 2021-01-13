#pragma once

#include <fstream>

#include "../Entity.h"
#include "../TileList.h"
#include "KoopaTroopa.h"

class Entity;

class Parakoopa : public KoopaTroopa {
private:	
	bool isOnGround;

	float runSpeed = 0.03f;
	float jumpSpeed = 0.22f;
	float gravity = 0.001f;

	void HandleStates() override;

public:
	Parakoopa();

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
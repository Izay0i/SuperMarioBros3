#pragma once

#include <fstream>

#include "../Entity.h"
#include "../TileList.h"
#include "KoopaTroopa.h"

class Entity;

class Parakoopa : public KoopaTroopa {
private:
	const float MIN_Y_OFFSET = 112.0f;

	bool isOnGround;

	float runSpeed = 0.03f;
	float jumpSpeed = 0.22f;
	float gravity = 0.001f;

	D3DXVECTOR3 originalPos;

	void HandleStates() override;

public:
	Parakoopa();

	void SetPosition(D3DXVECTOR3 pos) override { position = pos; originalPos = position; }

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
#pragma once

#include <fstream>

#include "../Entity.h"
#include "Boomerang.h"

class Entity;

class BoomerBro : public Entity {
private:
	enum class BroState {
		WALK,
		DIE
	};

	const float MAX_X_OFFSET = 8.0f;
	const float MIN_X_OFFSET = 8.0f;

	const static int MAX_FILE_LINE = 1024;

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	BroState currentState;

	float runSpeed = 0.03f;
	float gravity = 0.0002f;

	D3DXVECTOR3 originalPos;

	void LoadTexture();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

	void HandleStates();

public:
	BoomerBro();

	void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>()) override;

	RECTF GetBoundingBox(int = 0) const;

	Boomerang* SpawnBoomerang();

	void TakeDamage() override;

	void SetPosition(D3DXVECTOR3 pos) override { position = pos; originalPos = position; }

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
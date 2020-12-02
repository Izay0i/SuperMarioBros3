#pragma once

#include <fstream>

#include "../Entity.h"
#include "../TileList.h"

class Entity;

class Parakoopa : public Entity {
private:
	enum class ParakoopaState {
		FLY,
		WALK,
		RETRACT,
		SPIN,
		DIE
	};

	const static int MAX_FILE_LINE = 1024;

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	ParakoopaState currentState;

	bool isOnGround;

	float runSpeed = 0.03f;
	float jumpSpeed = 0.011f;
	float gravity = 0.001f;

	DWORD retractStart;
	DWORD retractTime = 8000;

	void LoadTexture();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

	void HandleStates();

public:
	Parakoopa();

	RECTF GetBoundingBox(int = 0) const override;

	void ParseData(std::string, std::string, D3DCOLOR) override;

	void StartRetract() { retractStart = static_cast<DWORD>(GetTickCount64()); }

	void TakeDamage() override;

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
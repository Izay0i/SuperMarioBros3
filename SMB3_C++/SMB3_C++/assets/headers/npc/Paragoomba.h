#pragma once

#include <fstream>

#include "../Entity.h"
#include "../TileList.h"

class Entity;

class Paragoomba : public Entity {
private:
	enum class ParagoombaState {
		FLY,
		WALK,
		DIE
	};

	const static int MAX_FILE_LINE = 1024;

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	ParagoombaState currentState;

	bool isOnGround;

	float runSpeed = 0.03f;
	float jumpSpeed = 0.006f;
	float gravity = 0.001f;

	void LoadTexture();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

	void HandleStates();

public:
	Paragoomba();

	RECTF GetBoundingBox(int = 0) const override;

	void ParseData(std::string, std::string, D3DCOLOR) override;

	void TakeDamage() override;

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
#pragma once

#include <fstream>

#include "../Entity.h"
#include "../TileList.h"

class Entity;

class Goomba : public Entity {
private:
	enum class GoombaState {
		WALK,
		DIE
	};

	const static int MAX_FILE_LINE = 1024;

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	GoombaState currentState;

	float runSpeed = 0.03f;
	float jumpSpeed = 0.5f;
	float gravity = 0.002f;

	void LoadTexture();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

	void HandleStates();

public:
	Goomba();

	RECTF GetBoundingBox(int = 0) const override;

	void ParseData(std::string, std::string, D3DCOLOR) override;
	
	void TakeDamage() override;

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
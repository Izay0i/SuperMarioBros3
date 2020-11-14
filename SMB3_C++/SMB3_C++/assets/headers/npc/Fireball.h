#pragma once

#include "../Entity.h"
#include "../TileList.h"

class Entity;

class Fireball : public Entity {
private:
	enum class BallState {
		BOUNCE,
		DIE
	};

	const static int MAX_FILE_LINE = 1024;

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	BallState currentState;
	
	float runSpeed = 0.2f;
	float jumpSpeed = 0.2f;
	float gravity = 0.002f;

	void LoadTexture();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

	void HandleStates();

public:
	Fireball();

	RECTF GetBoundingBox(int = 0) const override;

	void ParseData(std::string, std::string, D3DCOLOR) override;

	void TakeDamage() override;

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
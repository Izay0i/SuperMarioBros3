#pragma once

#include <fstream>

#include "../Entity.h"
#include "../TileList.h"

class Entity;

class Goomba : public Entity {
protected:
	enum class GoombaState {
		FLY,
		WALK,
		DIE
	};

	const static int MAX_FILE_LINE = 1024;

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	std::string animName = "Die";

	GoombaState currentState;

	float runSpeed = 0.03f;
	float jumpSpeed = 0.5f;
	float gravity = 0.002f;

	DWORD removeTime = 800;

	virtual void LoadTexture();

	virtual void ParseSprites(std::string);
	virtual void ParseHitboxes(std::string);

	virtual void HandleStates();

public:
	Goomba();

	virtual RECTF GetBoundingBox(int = 0) const override;

	virtual void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>()) override;

	virtual void TakeDamage() override;

	virtual void SetAnimationName(const std::string name) { animName = name; }

	virtual void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	virtual void Render() override;

	virtual void Release() override;
};
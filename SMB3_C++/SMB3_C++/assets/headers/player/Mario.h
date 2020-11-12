#pragma once

#include <fstream>

#include <dinput.h>

#include "../Entity.h"
#include "MarioStateMachine.h"

#define VK_A 0x41
#define VK_D 0x44
#define VK_K 0x4B
#define VK_S 0x53
#define VK_W 0x57

class Entity;
class MarioStateMachine;

class Mario : public Entity {
private:
	static Entity* marioInstance;

	const static int MAX_FILE_LINE = 1024;

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	MarioStateMachine* marioFSM;

	float runSpeed = 0.15f;
	float jumpSpeed = 0.5f;
	float gravity = 0.002f;
	float acceleration;

	void LoadTexture();

	void CheckCollision(Entity*, Entity*) override;

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

	Mario();

public:
	static Mario* GetInstance();

	bool IsOnGround() {}

	RECTF GetBoundingBox(int = 0) const override;
	AnimatedSprite GetSprite() { return sprite; }

	void ParseData(std::string, std::string, D3DCOLOR) override;
	
	void HandleStates(BYTE* states);
	void OnKeyDown(int);
	void OnKeyUp(int) {}

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
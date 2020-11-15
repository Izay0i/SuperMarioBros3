#pragma once

#include <fstream>

#include <dinput.h>

#include "../Entity.h"
#include "MarioStateMachine.h"

class Entity;
class Fireball;
class MarioStateMachine;

class Mario : public Entity {
private:
	static Entity* marioInstance;

	const static int MAX_FILE_LINE = 1024;

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	MarioStateMachine* marioFSM;

	std::string fireballPath;
	std::string texPath;

	bool isOnGround = true;

	const float maxAccel = 1.79f;

	float runSpeed = 0.09f;
	float jumpSpeed = 0.4f;
	float deflectSpeed = 0.3f;
	float dieflectSpeed = 0.7f;
	float gravity;
	float acceleration;

	void LoadTexture();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

	Mario();

public:

	static Mario* GetInstance();

	float GetAcceleration() { return acceleration; }

	bool IsOnGround() { return isOnGround; }

	RECTF GetBoundingBox(int = 0) const override;
	AnimatedSprite GetSprite() { return sprite; }

	void ParseData(std::string, std::string, std::string, D3DCOLOR);
	
	void HandleStates(BYTE*);
	void OnKeyDown(int);
	void OnKeyUp(int) {}

	Fireball* SpawnFireball();

	void TakeDamage() override;

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
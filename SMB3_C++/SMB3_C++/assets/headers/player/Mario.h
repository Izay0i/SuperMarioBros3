#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <fstream>

#include <dinput.h>

#include "../Entity.h"
#include "MarioStateMachine.h"

class Entity;
class Fireball;
class MarioStateMachine;

class Mario : public Entity {
private:
	static Mario* marioInstance;

	const static int MAX_FILE_LINE = 1024;

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	MarioStateMachine* marioFSM;
	Entity* heldEntity;

	bool isOnGround;
	bool isHolding;

	const float MAX_GRAVITY = 0.0005f;
	const float MAX_ACCEL = 2.2f;
	const float MIN_ACCEL = 1.1f; //for walking
	const float ACCEL_THRESHOLD = 1.86f; //how fast Mario should run before he can fly

	float runSpeed = 0.09f;
	float jumpSpeed = 0.33f;
	float deflectSpeed = 0.26f;
	float dieflectSpeed = 0.4f;
	float gravity = 0.0025f;
	float acceleration = 0.5f;

	void LoadTexture();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

	void HandleMovement();

	Mario();

public:

	static Mario* GetInstance();

	float GetAcceleration() const { return acceleration; }
	float GetAccelThreshold() const { return ACCEL_THRESHOLD; }

	bool IsOnGround() { return isOnGround; }

	RECTF GetBoundingBox(int = 0) const override;
	AnimatedSprite GetSprite() const { return sprite; }
	Entity* GetHeldEntity() { return heldEntity; }

	void ParseData(std::string, std::string, D3DCOLOR, std::string);
	
	void HandleStates(BYTE*);
	void OnKeyDown(int);
	void OnKeyUp(int) {}

	Fireball* SpawnFireball();

	void TakeDamage() override;

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
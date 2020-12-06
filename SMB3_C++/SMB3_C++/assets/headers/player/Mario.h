﻿#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <fstream>

#include <dinput.h>

#include "../Entity.h"
#include "MarioStateMachine.h"

class Entity;
class Fireball;
class MarioStateMachine;
class BonusItem;

//band-aid fixes baby
class Mario : public Entity {
private:
	static Mario* marioInstance;

	const static int MAX_FILE_LINE = 1024;

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	MarioStateMachine* marioFSM;
	Entity* heldEntity;

	std::vector<Fireball*> fireballs;

	//blue row
	std::vector<BonusItem*> bonusItems;
	//pink row
	std::vector<GameObject*> powerUps;

	bool isOnGround;
	bool isHolding;

	const float MAX_GRAVITY = 0.0005f;
	const float MAX_ACCEL = 2.2f;
	const float MIN_ACCEL = 1.1f; //for walking
	const float ACCEL_THRESHOLD = 1.86f; //how fast Mario should run before he can fly

	float runSpeed = 0.09f;
	float jumpSpeed = 0.327f;
	float deflectSpeed = 0.26f;
	float dieflectSpeed = 0.4f;
	float gravity = 0.0025f;
	float acceleration = 0.5f;

	DWORD attackStart;
	DWORD attackTime = 126;

	DWORD flyStart;
	DWORD flyTime = 8000;

	void LoadTexture();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

	void HandleMovement();

	Mario();

public:

	static Mario* GetInstance();

	float GetAcceleration() const { return acceleration; }
	float GetAccelThreshold() const { return ACCEL_THRESHOLD; }

	bool IsOnGround() const { return isOnGround; }
	bool IsAttacking() const { return attackStart != 0; }
	bool IsFlying() const { return flyStart != 0; }

	RECTF GetBoundingBox(int = 0) const override;
	AnimatedSprite GetSprite() const { return sprite; }
	Entity* GetHeldEntity() { return heldEntity; }

	void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>()) override;
	
	void HandleStates(BYTE*);
	void OnKeyDown(int);
	void OnKeyUp(int) {}

	Fireball* SpawnFireball();

	void StartAttackTimer() { attackStart = static_cast<DWORD>(GetTickCount64()); }
	void StartFlyTimer() { flyStart = static_cast<DWORD>(GetTickCount64()); }

	void TakeDamage() override;

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
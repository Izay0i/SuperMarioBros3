#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <fstream>

#include <dinput.h>

#include "../Entity.h"
#include "MarioStateMachine.h"

class Entity;
class Fireball;
class MarioStateMachine;

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
	Entity* touchedEntity;

	std::vector<Fireball*> fireballs;

	//retries
	unsigned int lives = 3;
	unsigned int coins;
	unsigned int score;

	//blue row	
	std::vector<Entity::ObjectType> bonusItems;

	bool triggeredStageEnd;

	bool isInMap;
	bool isOnGround;
	bool isHolding;

	const float MAX_GRAVITY = 0.0005f;
	const float MAX_ACCEL = 2.2f;
	const float MIN_ACCEL = 1.1f; //for walking
	const float ACCEL_THRESHOLD = 1.86f; //how fast Mario should run before he can fly

	float runSpeed = 0.09f;
	float jumpSpeed = 0.327f;
	float deflectSpeed = 0.4f;
	float gravity = 0.0025f;
	float acceleration = 0.5f;

	DWORD attackStart;
	DWORD attackTime = 126;

	DWORD flyStart;
	DWORD flyTime = 6000;

	DWORD invulStart;
	DWORD invulTime = 3000;

	void LoadTexture();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);
	void ParseSettings(std::string);

	void HandleMovement();
	void HandleStageEnd();
	void HandleBonusItems();

	Mario();

public:
	static Mario* GetInstance();

	unsigned int GetLivesLeft() const { return lives; }
	unsigned int GetCoinsCollected() const { return coins; }
	unsigned int GetCurrentScore() const { return score; }

	float GetAcceleration() const { return acceleration; }
	float GetAccelThreshold() const { return ACCEL_THRESHOLD; }
	float GetMaxAcceleration() const { return MAX_ACCEL; }

	std::vector<Entity::ObjectType> GetBonusItems() const { return bonusItems; }

	RECTF GetBoundingBox(int = 0) const override;
	AnimatedSprite GetSprite() const { return sprite; }
	Entity* GetHeldEntity() const { return heldEntity; }
	Entity* GetTouchedEntity() const { return touchedEntity; }

	bool TriggeredStageEnd() const { return triggeredStageEnd; }

	bool IsInMap() const { return isInMap; }
	bool IsRunningKeyPressed() const { return isHolding; }
	bool IsOnGround() const { return isOnGround; }
	bool IsAttacking() const { return attackStart != 0; }
	bool IsFlying() const { return flyStart != 0; }
	bool IsInvulnerable() const { return invulStart != 0; }

	void StartAttackTimer() { attackStart = static_cast<DWORD>(GetTickCount64()); }
	void StartFlyTimer() { flyStart = static_cast<DWORD>(GetTickCount64()); }
	void StartInvulTimer() { invulStart = static_cast<DWORD>(GetTickCount64()); }

	void GoRight();

	void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>()) override;
	
	void HandleStates(BYTE*);
	void OnKeyDown(int);
	void OnKeyUp(int) {}

	Fireball* SpawnFireball();

	void TakeDamage() override;

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
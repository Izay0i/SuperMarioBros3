#pragma once

#include "../Entity.h"
#include "state/PlayerState.h"
#include "Tail.h"

#include <fstream>

class Fireball;
class PlayerState;
class Player : public Entity {
private:
	friend class PlayerState;
	friend class MapState;
	friend class IdleState;
	friend class RunState;
	friend class JumpState;
	friend class FallState;
	friend class CrouchState;
	friend class ThrowState;
	friend class WagState;
	friend class HUD;
	friend class ScorePopUp;

	static Texture* _playerTexture;
	static std::vector<GameObjectType> _bonusItems;

	const unsigned int _DEFAULT_LIVES = 3;
	const unsigned int _MAX_LIVES = 99;
	const unsigned int _MAX_COINS = 99;
	const unsigned int _MAX_SCORE = 9999999;

	const unsigned int _FIREBALLS_LIMIT = 2;

	//Used only when the player is in the map scene
	const float _MAX_TRAVEL_DISTANCE = 31.0f;
	const float _MAX_GRAVITY = 0.0005f;
	const float _MAX_ACCEL = 2.2f;
	const float _MIN_ACCEL = 1.1f;
	const float _ACCEL_THRESHOLD = 1.86f;

	const float _CROUCH_HEIGHT_ADJUST = 8.0f;

	unsigned int _lives;
	unsigned int _coins;
	unsigned int _score;

	//How many fireballs the player can throw in a given time
	//Default is 2
	unsigned int _fireballsCount;
	unsigned int _nextSceneID;
	unsigned int _sceneRemainingTime;

	//Determines whether the player goes up or down in the pipe
	float _upVector;

	bool _triggeredStageEnd;
	bool _hasBossItem;
	bool _wentIntoPipe;
	bool _isCrouching;
	//Can be interpreted as holding an entity or pressing the K key
	bool _isHolding;
	bool _isNextToShell;

	Entity* _heldEntity;

	PlayerState* _playerState;

	D3DXVECTOR2 _lastPos;
	D3DXVECTOR2 _mapNodePos;
	D3DXVECTOR2 _destination;

	D3DXVECTOR2 _originalVel;

	DWORD _flyStart;
	DWORD _flyTime;

	DWORD _inPipeStart;
	DWORD _inPipeTime;

	//Tail attack
	DWORD _attackStart;
	DWORD _attackTime;

	//Fireball attack cooldown
	DWORD _fireballCoolDownStart;
	DWORD _fireballCoolDownTime;

	DWORD _invulnerableStart;
	DWORD _invulnerableTime;

	void _ParseSprites(std::string) override;
	//Lives, coins, score
	void _HandleCurrencies();
	void _HanldeStageEnd();
	void _HandleBonusItems();

	void _HandleMovementMap();
	void _HandleMovementGame();

public:
	bool isInMap;
	bool lockCameraXAxis;

	Player();
	~Player();

	unsigned int GetNextSceneID() const;
	RECTF GetBoundingBox(int = 0) const override;
	Entity* GetHeldEntity() const;

	void GetSceneRemainingTime(unsigned int);

	void SetUpVector(float);

	bool TriggeredStageEnd() const;
	bool WentIntoPipe() const;
	bool IsFlying() const;
	bool IsInPipe() const;
	bool IsAttacking() const;
	bool IsOnFireballCoolDown() const;
	bool IsInvulnerable() const;

	void StartFlyTimer();
	void StartInPipeTimer();
	void StartAttackTimer();
	void StartFireballCoolDownTimer();
	void StartInvulnerableTimer();

	void HandleStates() override;
	void OnKeyUpMap(int);
	void OnKeyUpGame(int);
	void OnKeyDownMap(int);
	void OnKeyDownGame(int);

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void TakeDamage() override;

	void MoveLeft();
	void MoveRight();
	void Jump();
	void RunFly();
	//Raccoon
	void SlowFall();
	
	Fireball* SpawnFireball();

	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;
	void HandleOverlap(Entity*) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
#pragma once

#include "../Entity.h"
#include "state/PlayerState.h"
#include "Tail.h"

#include <list>
#include <fstream>

class PlayerState;
class Player : public Entity {
private:
	friend class PlayerState;
	friend class IdleState;
	friend class RunState;
	friend class JumpState;
	friend class FallState;
	friend class CrouchState;
	friend class ThrowState;
	friend class WagState;
	friend class HUD;
	friend class Tail;

	static LPDIRECT3DTEXTURE9 _playerTexture;

	const unsigned int _DEFAULT_LIVES = 3;
	const unsigned int _MAX_LIVES = 99;
	const unsigned int _MAX_COINS = 99;
	const unsigned int _MAX_SCORE = 9999999;

	//Used only when the player is in the map scene
	const float _MAX_TRAVEL_DISTANCE = 31.0f;
	const float _MAX_GRAVITY = 0.0005f;
	const float _MAX_ACCEL = 2.2f;
	const float _MIN_ACCEL = 1.1f;
	const float _ACCEL_THRESHOLD = 1.86f;

	float _runSpeed;
	float _jumpSpeed;
	float _bounceSpeed;
	float _gravity;
	float _acceleration;

	unsigned int _lives;
	unsigned int _coins;
	unsigned int _score;

	bool _triggeredStageEnd;
	bool _wentIntoPipe;
	bool _isInMap;
	bool _isCrouching;
	//Can be interpreted as holding an entity or pressing the K key
	bool _isHolding;
	bool _isNextToShell;

	Entity* _heldEntity;
	Entity* _touchedEntity;

	PlayerState* _playerState;

	Tail* _tail;

	std::list<GameObject::GameObjectType> _bonusItems;
	//std::list<Fireball*> _fireballs;

	DWORD _flyStart;
	DWORD _flyTime;

	DWORD _inPipeStart;
	DWORD _inPipeTime;

	DWORD _attackStart;
	DWORD _attackTime;

	void _ParseSprites(std::string) override;

public:
	Player();
	~Player();

	RECTF GetBoundingBox(int = 0) const override;
	Entity* GetHeldEntity() const;
	
	bool TriggeredStageEnd() const;
	bool WentIntoPipe() const;
	bool IsFlying() const;
	bool IsInPipe() const;
	bool IsAttacking() const;

	void HandleStates() override;
	void OnKeyUp(int);
	void OnKeyDown(int);

	void ParseData(std::string, const LPDIRECT3DTEXTURE9&, std::vector<std::string> = std::vector<std::string>()) override;

	void MoveLeft();
	void MoveRight();
	void Jump();
	void RunFly();
	//Raccoon
	void SlowFall();
	
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
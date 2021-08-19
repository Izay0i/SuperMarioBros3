#pragma once

#include "../Entity.h"
#include "StateMachine.h"

#include <list>
#include <fstream>

class StateMachine;

class Player : public Entity {
private:
	static LPDIRECT3DTEXTURE9 _playerTexture;

	//Used only when the player is in the map scene
	const float _MAX_TRAVEL_DISTANCE = 31.0f;

	const unsigned int _DEFAULT_LIVES = 3;
	const unsigned int _MAX_LIVES = 99;
	const unsigned int _MAX_COINs = 99;
	const unsigned int _MAX_SCORE = 9999999;

	const float _MAX_GRAVITY = 0.0005f;
	const float _MAX_ACCEL = 2.2f;
	const float _MIN_ACCEL = 1.1f;
	const float _ACCEL_THRESHOLD = 1.86f;

	float _runSpeed;
	float _jumpSpeed;
	float _bounceSpeed;
	float _gravity;
	float _acceleration;

	bool _isOnGround;

	StateMachine* _stateMachine;

	Entity* _heldEntity;
	Entity* _touchedEntity;

	//std::list<Fireball*> _fireballs;

	void _ParseSprites(std::string) override;

public:
	Player();
	~Player();

	RECTF GetBoundingBox(int = 0) const override;
	Entity* GetHeldEntity() const;

	bool IsOnGround() const;

	void HandleStates(int, bool) override;
	void OnKeyUp(int);
	void OnKeyDown(int);

	void ParseData(std::string, const LPDIRECT3DTEXTURE9&, std::vector<std::string> = std::vector<std::string>()) override;

	void MoveLeft();
	void MoveRight();
	void SkidLeft();
	void SkidRight();
	void Jump();
	void Crouch();
	void HoldEntity();
	
	void HandleCollisionEventResults(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
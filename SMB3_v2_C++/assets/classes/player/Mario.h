#pragma once

#include "../Device.h"
#include "../Entity.h"

#include <list>
#include <fstream>

class Mario : public Entity {
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

	//StateMachine* _stateMachine;

	Entity* _heldEntity;
	Entity* _touchedEntity;

	//std::list<Fireball*> _fireballs;

	void _ParseSprites(std::string) override;

public:
	Mario();
	~Mario();

	RECTF GetBoundingBox(int = 0) const override;

	void HandleStates() override;
	void OnKeyUp(int);
	void OnKeyDown(int);

	void ParseData(std::string, const LPDIRECT3DTEXTURE9&, std::vector<std::string> = std::vector<std::string>()) override;

	void MoveLeft();
	void MoveRight();
	void Jump();
	void Crouch();
	void HoldEntity();

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
#pragma once

#include <fstream>

#include "../Entity.h"

#define VK_A 0x41
#define VK_D 0x44
#define VK_K 0x4B
#define VK_S 0x53
#define VK_W 0x57

class Entity;

class Mario : public Entity {
private:
	enum class MarioForm {
		SMALL,
		BIG,
		FIRE,
		RACOON,

		//Out of your friends
		//Which one are you?
		TruckFreak,
		CrazyAss,
		друг,
		TheFighter,

		Trickster,
		Swordmaster,
		Gunslinger,
		Royalguard	
	};

	//Some states only available in certain forms
	enum class MarioState {
		IDLE,
		RUN,
		TURN,
		JUMP,
		FALL,
		FRONT,
		HOLD,
		KICK,
		SLIDE,
		SWIM,
		CLIMB,
		DIE,
		//BIG, FIRE, RACOON
		CROUCH,
		BACK,
		//FIRE
		SHOOT,
		//RACOON
		SPIN
	};

	static Entity* marioInstance;

	const static int MAX_FILE_LINE = 1024;

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	float runSpeed = 0.10f;
	float jumpSpeed = 0.2f;
	float gravity = 0.002f;

	MarioForm currentForm;
	MarioState currentState;

	void LoadTexture();

	void CheckCollision(Entity*, Entity*) override;
	
	void HandleStates();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

	Mario();

public:
	static Mario* GetInstance();

	bool IsOnGround() {}

	void ParseData(std::string, std::string, D3DCOLOR) override;

	void SetMarioForm(MarioForm form) { currentForm = form; }
	MarioForm GetMarioForm() { return currentForm; }

	void SetState(MarioState state) { currentState = state; }
	MarioState GetState() { return currentState; }

	void OnKeyDown(int);
	void OnKeyUp(int);

	void Update(DWORD) override;
	void Render() override;

	void Release() override;
};
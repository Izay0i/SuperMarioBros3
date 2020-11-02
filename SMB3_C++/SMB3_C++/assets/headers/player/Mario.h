#pragma once

#include <fstream>

#include "../StateMachine.h"
#include "../Entity.h"

class Entity;

class Mario : Entity {
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

	static Entity* marioInstance;

	const static int MAX_FILE_LINE = 1024;

	void CheckCollision(Entity*, Entity*) override;

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

	Mario();

public:
	static Mario* GetInstance();

	bool IsOnGround() {}

	void ParseData(std::string);

	void SetPosition(D3DXVECTOR3) override;
	D3DXVECTOR3 GetPosition() override;	

	void SetRotation(D3DXVECTOR2) override;
	D3DXVECTOR2 GetRotation() override;

	void SetTranslation(D3DXVECTOR2) override;
	D3DXVECTOR2 GetTranslation() override;

	void SetScale(D3DXVECTOR2) override;
	D3DXVECTOR2 GetScale() override;

	void Update(DWORD) override;
	void Render() override;

	void Release() override;
};

class MarioStateMachine : StateMachine {
private:
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
};
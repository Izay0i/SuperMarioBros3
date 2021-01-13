#pragma once

#include "GameObject.h"
#include "AnimatedSprite.h"

class AnimatedSprite;

class Entity : public GameObject {
protected:
	enum class DataSection {
		DATA_SECTION_UNKNOWN,
		DATA_SECTION_SPRITES,
		DATA_SECTION_HITBOXES
	};

	std::vector<std::string> extraData;

	bool isBeingHeld = false;

	//-1: flag for removal
	int hitPoints = 1;

	DWORD removeStart;
	//Implement your own removeTime

	AnimatedSprite sprite;

public:
	enum class ObjectType {
		//entities
		OBJECT_TYPE_MARIO = 0,
		OBJECT_TYPE_GOOMBA = 1,
		OBJECT_TYPE_PARAGOOMBA = 2,
		OBJECT_TYPE_TROOPA = 3,
		OBJECT_TYPE_PARATROOPA = 4,
		OBJECT_TYPE_PIPLANT = 5,
		OBJECT_TYPE_VENUSTRAP = 6,
		OBJECT_TYPE_HAMMERBRO = 7,
		//power-ups
		OBJECT_TYPE_MUSHROOM = 8,
		OBJECT_TYPE_1UPSHROOM = 9,
		OBJECT_TYPE_LEAF = 10,
		//animated tiles
		OBJECT_TYPE_COIN = 101,
		OBJECT_TYPE_QUESTIONBLOCK = 102,
		OBJECT_TYPE_SHINYBRICK = 103,
		OBJECT_TYPE_BONUSITEM = 104,
		OBJECT_TYPE_SWITCHBLOCK = 105,
		//overworld map
		OBJECT_TYPE_CACTUS = 106,
		OBJECT_TYPE_HELP = 107,
		OBJECT_TYPE_HAMMERBRONODE = 108
	};

	virtual ~Entity() {}	

	void SetCurrenHitPoints(int point) { hitPoints = point; }
	int GetCurrentHitPoints() const { return hitPoints; }

	virtual unsigned int GetExtraDataSize() const { return extraData.size(); }

	virtual void SetStatus(bool b) { isBeingHeld = b; }
	virtual bool IsBeingHeld() const { return isBeingHeld; }

	virtual void StartRemoveTimer() { removeStart = static_cast<DWORD>(GetTickCount64()); }
	virtual bool IsBeingRemoved() { return removeStart != 0; }

	virtual void TakeDamage() {}

	virtual void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>()) {}

	virtual void Update(DWORD, std::vector<GameObject*>* = nullptr) = 0;
	virtual void Render() = 0;

	virtual void Release() = 0;
};
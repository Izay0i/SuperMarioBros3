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

	//-1: flag for removal
	int hitPoints = 1;

	DWORD removeStart;
	//Implement your own removeTime

	AnimatedSprite sprite;

public:
	virtual ~Entity() {}	

	void SetCurrenHitPoints(int point) { hitPoints = point; }
	int GetCurrentHitPoints() const { return hitPoints; }

	virtual void StartRemoveTimer() { removeStart = static_cast<DWORD>(GetTickCount64()); }

	virtual void TakeDamage() {}

	virtual void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>()) {}

	virtual void Update(DWORD, std::vector<GameObject*>* = nullptr) = 0;
	virtual void Render() = 0;

	virtual void Release() = 0;
};
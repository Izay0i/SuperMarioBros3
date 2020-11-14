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

	int hitPoints;

	AnimatedSprite sprite;

public:
	virtual ~Entity() {}	

	int GetCurrentHitPoints() const { return hitPoints; }

	virtual void TakeDamage() {}

	virtual void ParseData(std::string, std::string, D3DCOLOR) {}

	virtual void Update(DWORD, std::vector<GameObject*>* = nullptr) = 0;
	virtual void Render() = 0;

	virtual void Release() = 0;
};
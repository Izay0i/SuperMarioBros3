#pragma once

#include "GameObject.h"
#include "AnimatedSprite.h"
#include "HitBox.h"

class AnimatedSprite;
class HitBox;

class Entity : public GameObject {
protected:
	enum class DataSection {
		DATA_SECTION_UNKNOWN,
		DATA_SECTION_SPRITES,
		DATA_SECTION_HITBOXES
	};

	AnimatedSprite* sprite;
	HitBox hitBox;

	virtual void CheckCollision(Entity*, Entity*) = 0;

public:
	virtual ~Entity() {}	

	virtual void ParseData(std::string, std::string, D3DCOLOR) {}

	virtual void Update(DWORD) = 0;
	virtual void Render() = 0;

	virtual void Release() = 0;
};
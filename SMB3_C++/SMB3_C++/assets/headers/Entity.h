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

	AnimatedSprite sprite;

	virtual void CheckCollision(Entity*, Entity*) = 0;

public:
	virtual ~Entity() {}	

	virtual void ParseData(std::string, std::string, D3DCOLOR) = 0;

	virtual void Update(DWORD, std::vector<GameObject*>* = nullptr) = 0;
	virtual void Render() = 0;

	virtual void Release() = 0;
};
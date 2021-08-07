#pragma once

#include "GameObject.h"
#include "AnimatedSprite.h"

class Entity : public GameObject {
protected:
	enum class _DataFileSection {
		DATAFILE_SECTION_UNKNOWN,
		DATAFILE_SECTION_SPRITES,
		DATAFILE_SECTION_HITBOXES,
		DATAFILE_SECTION_SETTINGS
	};

	std::vector<std::string> _extraData;

	bool _isOnGround;
	bool _isHeld;
	bool _tookDamage;

	//-1: set to inactive
	int _health;
	
	unsigned int _score, _scoreMultiplier;

	AnimatedSprite _animatedSprite;

public:
	Entity();
	virtual ~Entity();

	virtual void SetHealth(int);
	virtual int GetHealth() const;

	virtual void LoadTexture() = 0;

	virtual void ParseSprites(std::string) = 0;
	virtual void ParseHitboxes(std::string);
	//Entity data  file path, texture file path, color key, extra data/properties
	virtual void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>());

	virtual void HandleStates() = 0;
};
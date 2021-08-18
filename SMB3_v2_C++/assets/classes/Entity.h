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
	
	//The lower the value, the higher the render priority;
	unsigned int _renderPriority;
	unsigned int _score, _scoreMultiplier;

	AnimatedSprite _animatedSprite;

	virtual void _ParseSprites(std::string) = 0;
	virtual void _ParseHitboxes(std::string);

public:
	static bool CompareRenderPriority(const Entity*&, const Entity*&);

	Entity();
	virtual ~Entity();

	virtual void SetHealth(int);
	virtual int GetHealth() const;

	virtual AnimatedSprite GetAnimatedSprite() const;

	//Entity data  file path, texture, extra data/properties
	virtual void ParseData(std::string, const LPDIRECT3DTEXTURE9&, std::vector<std::string> = std::vector<std::string>());

	virtual void HandleStates(int, bool) = 0;
};
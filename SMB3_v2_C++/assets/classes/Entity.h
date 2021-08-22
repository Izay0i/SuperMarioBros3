#pragma once

#include "GameObject.h"
#include "AnimatedSprite.h"

class Grid;
struct Cell;
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
	unsigned int cellVectorIndex;
	Cell* ownerCell;

	static bool CompareRenderPriority(Entity*&, Entity*&);

	Entity();
	virtual ~Entity();

	virtual void SetHealth(int);
	virtual int GetHealth() const;

	//Entity data  file path, texture, extra data/properties
	virtual void ParseData(std::string, const LPDIRECT3DTEXTURE9&, std::vector<std::string> = std::vector<std::string>());

	virtual void HandleStates(int, bool) = 0;
	//Event results, min time, offset, normal, relative distance
	virtual void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) = 0;
	//Delta time, collidable entities, collidable tiles, grid
	virtual void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr);

	CollisionEvent* SweptAABBEx(Entity*&);
	void CalcPotentialCollision(std::vector<Entity*>*, std::vector<LPCOLLISIONEVENT>&);
};
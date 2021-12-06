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

	float _runSpeed;
	float _jumpSpeed;
	float _bounceSpeed;
	float _gravity;
	float _acceleration;

	bool _isOnGround;
	bool _isHeld;

	//-1: set to inactive
	int _health;
	
	//The lower the value, the higher the render priority;
	int _renderPriority;

	std::string _variant;

	AnimatedSprite _animatedSprite;

	DWORD _removeStart;
	//Default is 2s
	DWORD _removeTime;

	virtual void _ParseSprites(std::string) = 0;
	virtual void _ParseHitboxes(std::string);

public:
	bool tookDamage;
	bool isBeingHeld;

	int cellIndex;
	Cell* ownerCell;

	static bool CompareRenderPriority(Entity*&, Entity*&);

	Entity();
	virtual ~Entity();

	

	bool IsRemoved() const;
	void StartRemoveTimer();

	std::vector<std::string> GetExtraData() const;
	
	void SetGravity(float);
	float GetGravity() const;

	virtual void SetHealth(int);
	virtual int GetHealth() const;

	//Entity data  file path, texture, extra data/properties
	virtual void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>());

	virtual void TakeDamage();

	virtual void HandleStates() = 0;
	//Event results, min time, offset, normal, relative distance
	virtual void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) = 0;
	virtual void HandleOverlap(Entity*);
	//Delta time, collidable entities, collidable tiles, grid
	virtual void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr);

	//An extension of the SweptAABB method
	CollisionEvent* SweptAABBEx(Entity*&);
	//Calculate potential collision between entities
	void CalcPotentialCollision(std::vector<Entity*>*, std::vector<LPCOLLISIONEVENT>&);
};
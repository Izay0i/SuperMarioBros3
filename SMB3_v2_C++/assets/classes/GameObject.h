#pragma once

#define NOMINMAX

#include "Hitbox.h"

#include <fstream>
#include <algorithm>

#include <d3dx9.h>

class GameObject;

struct CollisionEvent;
typedef CollisionEvent* LPCOLLISIONEVENT;
struct CollisionEvent {
	GameObject* gameOject;
	D3DXVECTOR2 normal;
	D3DXVECTOR2 distance;
	float time;

	CollisionEvent(GameObject*& obj, D3DXVECTOR2 norm, D3DXVECTOR2 dist, float t) {
		gameOject = obj;
		normal = norm;
		distance = dist;
		time = t;
	}

	static bool CompareCollisionEvent(const LPCOLLISIONEVENT& a, const LPCOLLISIONEVENT& b) {
		return a->time < b->time;
	}
};

class GameObject {
public:
	//Max value of UINT 4294967295 (0xffffffff)
	enum class GameObjectType {
		//Players
		GAMEOBJECT_TYPE_MARIO = 0,
		GAMEOBJECT_TYPE_LUIGI = 1,
		//NPCs
		GAMEOBJECT_TYPE_GOOMBA = 100,
		GAMEOBJECT_TYPE_PARAGOOMBA = 101,
		GAMEOBJECT_TYPE_KOOPA = 102,
		GAMEOBJECT_TYPE_PARAKOOPA = 103,
		GAMEOBJECT_TYPE_PIRAPLANT = 104,
		GAMEOBJECT_TYPE_VENUSPLANT = 105,
		GAMEOBJECT_TYPE_BOOMERBRO = 106,
		//Special entities
		GAMEOBJECT_TYPE_PORTAL = 198,
		GAMEOBJECT_TYPE_MOVINGPLATFORM = 199,
		//Items
		GAMEOBJECT_TYPE_RMUSHROOM = 200,
		GAMEOBJECT_TYPE_GMUSHROOM = 201,
		GAMEOBJECT_TYPE_LEAF = 202,
		GAMEOBJECT_TYPE_FLOWER = 203,
		GAMEOBJECT_TYPE_COIN = 204,
		//Animated blocks
		GAMEOBJECT_TYPE_QUESTIONBLOCK = 300,
		GAMEOBJECT_TYPE_SHINYBRICK = 301,
		GAMEOBJECT_TYPE_BONUSITEM = 302,
		GAMEOBJECT_TYPE_PBLOCK = 303,
		//Overworld NPCs
		GAMEOBJECT_TYPE_CACTUS = 400,
		GAMEOBJECT_TYPE_HELPTEXT = 401,
		GAMEOBJECT_TYPE_HAMMERBRO = 402,
		//Intro items
		GAMEOBJECT_TYPE_LOGO = 500,
		GAMEOBJECT_TYPE_SELECT = 501,
		GAMEOBJECT_TYPE_CURTAIN = 502,
		//Special blocks
		GAMEOBJECT_TYPE_ONEHITPLATFORM = 666,
		GAMEOBJECT_TYPE_ONEWAYPLATFORM = 999
	};

protected:
	bool _isActive;

	GameObjectType _objectType;

	Hitbox _hitbox;

	DWORD _deltaTime;

	D3DXVECTOR2 _velocity;
	D3DXVECTOR2 _distance;
	D3DXVECTOR2 _normal;

	D3DXVECTOR2 _position;
	D3DXVECTOR2 _rotation;
	D3DXVECTOR2 _translation;
	D3DXVECTOR2 _scale;

	LPCWSTR _texturePath;
	D3DCOLOR _colorKey;

public:
	GameObject();
	virtual ~GameObject();

	void SetOjectType(GameObjectType);
	GameObjectType GetObjectType() const;

	virtual RECTF GetBoundingBox(int = 0) const;
	float GetBoxWidth(int = 0) const;
	float GetBoxHeight(int = 0) const;

	virtual void SetVelocity(D3DXVECTOR2);
	D3DXVECTOR2 GetVelocity() const;
	virtual void SetDistance(D3DXVECTOR2);
	D3DXVECTOR2 GetDistance() const;
	virtual void SetNormal(D3DXVECTOR2);
	D3DXVECTOR2 GetNormal() const;
	virtual void SetPosition(D3DXVECTOR2);
	D3DXVECTOR2 GetPosition() const;
	virtual void SetRotation(D3DXVECTOR2);
	D3DXVECTOR2 GetRotation() const;
	virtual void SetTranslation(D3DXVECTOR2);
	D3DXVECTOR2 GetTranslation() const;
	virtual void SetScale(D3DXVECTOR2);
	D3DXVECTOR2 GetScale() const;

	//Moving object, static object, distance of moving object, normal, time
	void SweptAABB(RECTF, RECTF, D3DXVECTOR2, D3DXVECTOR2&, float&);
	//An extension of the SweptAABB method
	CollisionEvent* SweptAABBEx(GameObject*&);

	//Calculate potential collision between objects
	void CalcPotentialCollision(std::vector<GameObject*>*&, std::vector<LPCOLLISIONEVENT>&);
	//Filter the collision between objects based on the shortest time
	void FilterCollision(const std::vector<LPCOLLISIONEVENT>&, std::vector<LPCOLLISIONEVENT>&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&);

	virtual void Update(DWORD, std::vector<GameObject*>* = nullptr);
	virtual void Render();

	virtual void Release() = 0;
};
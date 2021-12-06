#pragma once

#define NOMINMAX

#include "Hitbox.h"

#include <fstream>
#include <algorithm>

#include <d3dx9.h>

class GameObject;
class Entity;

struct CollisionEvent;
typedef CollisionEvent* LPCOLLISIONEVENT;
struct CollisionEvent {
	GameObject* gameOject;
	Entity* entity;
	D3DXVECTOR2 normal;
	D3DXVECTOR2 distance;
	float time;

	CollisionEvent(GameObject*& obj, D3DXVECTOR2 norm, D3DXVECTOR2 dist, float t) {
		gameOject = obj;
		normal = norm;
		distance = dist;
		time = t;
	}

	CollisionEvent(Entity*& ent, D3DXVECTOR2 norm, D3DXVECTOR2 dist, float t) {
		entity = ent;
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
	enum class GameObjectType {
		//Players
		GAMEOBJECT_TYPE_MARIO = 0,
		GAMEOBJECT_TYPE_LUIGI = 1,
		//NPCs
		GAMEOBJECT_TYPE_GOOMBA = 100,
		GAMEOBJECT_TYPE_PARAGOOMBA = 101,
		GAMEOBJECT_TYPE_KOOPA = 102,
		GAMEOBJECT_TYPE_PARAKOOPA = 103,
		GAMEOBJECT_TYPE_PIRANHAPLANT = 104,
		GAMEOBJECT_TYPE_VENUSPLANT = 105,
		GAMEOBJECT_TYPE_BOOMERANGBRO = 106,
		//Projectiles
		GAMEOBJECT_TYPE_PLAYERFIREBALL = 107,
		GAMEOBJECT_TYPE_VENUSFIREBALL = 108,
		GAMEOBJECT_TYPE_BOOMERANG = 109,
		GAMEOBJECT_TYPE_PODOBOO = 110,
		//NPCS
		GAMEOBJECT_TYPE_DRYBONES = 111,
		GAMEOBJECT_TYPE_ROTODISC = 112,
		//Special entities
		GAMEOBJECT_TYPE_TAIL = 197,
		GAMEOBJECT_TYPE_PORTAL = 198,
		GAMEOBJECT_TYPE_MOVINGPLATFORM = 199,
		//Items
		GAMEOBJECT_TYPE_REDMUSHROOM = 200,
		GAMEOBJECT_TYPE_GREENMUSHROOM = 201,
		GAMEOBJECT_TYPE_LEAF = 202,
		GAMEOBJECT_TYPE_FLOWER = 203,
		GAMEOBJECT_TYPE_STAR = 204,
		GAMEOBJECT_TYPE_COIN = 205,
		GAMEOBJECT_TYPE_BONUSITEM = 206,
		GAMEOBJECT_TYPE_ORB = 207,
		//Animated blocks
		GAMEOBJECT_TYPE_QUESTIONBLOCK = 300,
		GAMEOBJECT_TYPE_SHINYBRICK = 301,
		GAMEOBJECT_TYPE_PBLOCK = 302,
		GAMEOBJECT_TYPE_LAVAPOOL = 303,
		GAMEOBJECT_TYPE_DOOR = 304,
		//Overworld NPCs
		GAMEOBJECT_TYPE_CACTUS = 400,
		GAMEOBJECT_TYPE_HELPTEXT = 401,
		GAMEOBJECT_TYPE_HAMMERBRO = 402,
		//Space-time
		GAMEOBJECT_TYPE_DIALGA = 483,
		GAMEOBJECT_TYPE_PALKIA = 484,
		GAMEOBJECT_TYPE_GIRATINA = 487,
		//Intro items
		GAMEOBJECT_TYPE_LOGO = 500,
		GAMEOBJECT_TYPE_ICON = 501,
		GAMEOBJECT_TYPE_SELECT = 502,
		GAMEOBJECT_TYPE_CURTAIN = 503,
		GAMEOBJECT_TYPE_PROPPLANT = 504,
		GAMEOBJECT_TYPE_BUZZYBEETLE = 505,
		GAMEOBJECT_TYPE_PROPMARIO = 506,
		GAMEOBJECT_TYPE_PROPLUIGI = 507,
		GAMEOBJECT_TYPE_PROPFASTKOOPA = 508,
		GAMEOBJECT_TYPE_PROPKOOPASHELL = 509,
		GAMEOBJECT_TYPE_PROPNORMALKOOPA = 510,
		//Effects
		GAMEOBJECT_TYPE_SCOREEFFECT = 600,
		GAMEOBJECT_TYPE_BRICKEFFECT = 601,
		GAMEOBJECT_TYPE_SMOKEEFFECT = 602,
		GAMEOBJECT_TYPE_SPARKEFFECT = 603,
		GAMEOBJECT_TYPE_ORBEFFECT = 604,
		//Bosses
		GAMEOBJECT_TYPE_FORTRESSBOSS = 700,
		//Special objects
		GAMEOBJECT_TYPE_TRIGGER = 444,
		GAMEOBJECT_TYPE_MASKTILE = 555,
		GAMEOBJECT_TYPE_ONEHITPLATFORM = 666,
		GAMEOBJECT_TYPE_TILE = 777,
		GAMEOBJECT_TYPE_MOVINGCEILING = 888,
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

public:
	//If true, the entity is removed from the container
	//But its resources will not be released
	//Useful for when an object needs to know when another object is ready to be removed
	bool flaggedForRemoval;
	//Pass throughable entities are entities that
	//Other entities can pass through without blocking their velocity
	bool isPassThroughable;

	GameObject();
	virtual ~GameObject();

	//Is this Java?
	void SetActive(bool);
	bool IsActive() const;

	bool IsOverlapped(GameObject*) const;

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
	//Filter the collision between objects based on the shortest time
	void FilterCollision(const std::vector<LPCOLLISIONEVENT>&, std::vector<LPCOLLISIONEVENT>&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&);

	virtual void Update(DWORD, std::vector<GameObject*>* = nullptr);
	virtual void Render();

	virtual void Release() = 0;
};
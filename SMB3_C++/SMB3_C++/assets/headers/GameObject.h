#pragma once
#define NOMINMAX

#include <algorithm>

#include <d3dx9.h>

#include "Util.h"
#include "HitBox.h"

class GameObject;
class HitBox;

struct CollisionEvent;
typedef CollisionEvent* LPCOLLISIONEVENT;
struct CollisionEvent {
	GameObject* object;
	D3DXVECTOR3 normal;
	float time;

	D3DXVECTOR3 distance;

	CollisionEvent(float t, D3DXVECTOR3 norm, D3DXVECTOR3 dist = D3DXVECTOR3(0, 0, 0), GameObject* obj = nullptr) {
		time = t;
		normal = norm;
		distance = dist;
		object = obj;
	}

	static bool CompareCollisionEvent(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b) {
		return a->time < b->time;
	}
};

class GameObject {
protected:
	int objectID;
	
	HitBox hitBox;

	DWORD delta;

	D3DXVECTOR3 velocity;
	D3DXVECTOR3 distance;

	D3DXVECTOR3 position;
	D3DXVECTOR2 rotation;
	D3DXVECTOR2 translation;
	D3DXVECTOR2 scale;

	D3DXMATRIX matrix;

	static LPDIRECT3DDEVICE9 directDevice;
	static LPD3DXSPRITE spriteHandler;
	
public:
	GameObject() {}
	virtual ~GameObject() {}

	static void SweptAABB(RECTF, RECTF, D3DXVECTOR3, D3DXVECTOR3&, float&);
	CollisionEvent* SweptAABBEx(GameObject*);

	void CalcPotentialCollision(std::vector<GameObject*>*, std::vector<LPCOLLISIONEVENT>&);
	void FilterCollision(std::vector<LPCOLLISIONEVENT>&, std::vector<LPCOLLISIONEVENT>&, D3DXVECTOR2&, D3DXVECTOR3&, D3DXVECTOR3&);

	static void SetDevice(LPDIRECT3DDEVICE9&);
	static LPDIRECT3DDEVICE9 GetDevice();

	static void SetSpriteHandler(LPD3DXSPRITE&);
	static LPD3DXSPRITE GetSpriteHandler();

	void SetObjectID(int id) { objectID = id; }
	int GetObjectID() { return objectID; }

	virtual RECTF GetBoundingBox(int = 0) const = 0;
	unsigned int GetBoxWidth(int id = 0) { return hitBox.GetWidth(id); }
	unsigned int GetBoxHeight(int id = 0) { return hitBox.GetHeight(id); }

	virtual void SetVelocity(D3DXVECTOR3 vel) { velocity = vel; }
	virtual D3DXVECTOR3 GetVelocity() const { return velocity; }

	virtual void SetDistance(D3DXVECTOR3 dis) { distance = dis; }
	virtual D3DXVECTOR3 GetDistance() const { return distance; }

	virtual	void SetPosition(D3DXVECTOR3 pos) { position = pos; }
	virtual	D3DXVECTOR3 GetPosition() const { return position; }

	virtual	void SetRotation(D3DXVECTOR2 rot) { rotation = rot; }
	virtual	D3DXVECTOR2 GetRotation() const { return rotation; }

	virtual	void SetTranslation(D3DXVECTOR2 trans) { translation = trans; }
	virtual	D3DXVECTOR2 GetTranslation() const { return translation; }

	virtual	void SetScale(D3DXVECTOR2 sc) { scale = sc; }
	virtual	D3DXVECTOR2 GetScale() const { return scale; }

	virtual void Update(DWORD, std::vector<GameObject*>* = nullptr);
	virtual void Render() = 0;

	virtual void Release() = 0;
};
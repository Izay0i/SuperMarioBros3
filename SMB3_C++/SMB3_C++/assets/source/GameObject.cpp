#include "../headers/GameObject.h"

LPDIRECT3DDEVICE9 GameObject::directDevice = nullptr;
LPD3DXSPRITE GameObject::spriteHandler = nullptr;

void GameObject::SweptAABB(RECTF movingObject, RECTF staticObject, D3DXVECTOR3 distance, D3DXVECTOR3& normal, float& t) {
	D3DXVECTOR3 dEntry;
	D3DXVECTOR3 dExit;

	D3DXVECTOR3 tEntry;
	D3DXVECTOR3 tExit;

	float entry;
	float exit;

	t = -1.0f;
	normal = D3DXVECTOR3(0, 0, 0);

	//broad phase test
	RECTF box;
	box.left = distance.x > 0 ? movingObject.left : movingObject.left + distance.x;
	box.top = distance.y > 0 ? movingObject.top : movingObject.top + distance.y;
	box.right = distance.x > 0 ? movingObject.right + distance.x : movingObject.right;
	box.bottom = distance.y > 0 ? movingObject.bottom + distance.y : movingObject.bottom;

	if (box.right < staticObject.left || 
		box.left > staticObject.right || 
		box.bottom < staticObject.top || 
		box.top > staticObject.bottom) 
	{
		return;
	}

	if (distance == D3DXVECTOR3(0, 0, 0)) {
		return;
	}

	if (distance.x > 0) {
		dEntry.x = staticObject.left - movingObject.right;
		dExit.x = staticObject.right - movingObject.left;
	}
	else if (distance.x < 0) {
		dEntry.x = staticObject.right - movingObject.left;
		dExit.x = staticObject.left - movingObject.right;
	}

	if (distance.y > 0) {
		dEntry.y = staticObject.top - movingObject.bottom;
		dExit.y = staticObject.bottom - movingObject.top;
	}
	else if (distance.y < 0) {
		dEntry.y = staticObject.bottom - movingObject.top;
		dExit.y = staticObject.top - movingObject.bottom;
	}

	if (distance.x == 0) {
		tEntry.x = -std::numeric_limits<float>::infinity();
		tExit.x = std::numeric_limits<float>::infinity();
	}
	else {
		tEntry.x = dEntry.x / distance.x;
		tExit.x = dExit.x / distance.x;
	}

	if (distance.y == 0) {
		tEntry.y = -std::numeric_limits<float>::infinity();
		tExit.y = std::numeric_limits<float>::infinity();
	}
	else {
		tEntry.y = dEntry.y / distance.y;
		tExit.y = dExit.y / distance.y;
	}

	if ((tEntry.x < 0.0f && tEntry.y < 0.0f) || 
		tEntry.x > 1.0f || 
		tEntry.y > 1.0f) 
	{
		return;
	}

	entry = std::max(tEntry.x, tEntry.y);
	exit = std::min(tExit.x, tExit.y);

	if (entry > exit) {
		return;
	}

	t = entry;

	if (tEntry.x > tEntry.y) {
		normal.y = 0.0f;
		distance.x > 0 ? normal.x = -1.0f : normal.x = 1.0f;
	}
	else {
		normal.x = 0.0f;
		distance.y > 0 ? normal.y = -1.0f : normal.y = 1.0f;
	}
}

CollisionEvent* GameObject::SweptAABBEx(GameObject* object) {
	RECTF staticObject;
	RECTF movingObject;
	D3DXVECTOR3 normal;
	float t;

	return nullptr;
}

void GameObject::SetDevice(LPDIRECT3DDEVICE9& dev) { 
	if (!directDevice) { 
		directDevice = dev; 
	} 
}

LPDIRECT3DDEVICE9 GameObject::GetDevice() { 
	return directDevice; 
}

void GameObject::SetSpriteHandler(LPD3DXSPRITE& handler) { 
	if (!spriteHandler) { 
		spriteHandler = handler; 
	} 
}

LPD3DXSPRITE GameObject::GetSpriteHandler(){ 
	return spriteHandler; 
}
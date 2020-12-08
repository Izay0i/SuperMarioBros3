#include "../headers/GameObject.h"

LPDIRECT3DDEVICE9 GameObject::directDevice = nullptr;
LPD3DXSPRITE GameObject::spriteHandler = nullptr;

void GameObject::Update(DWORD delta, std::vector<GameObject*>* objects) {
	this->delta = delta;
	distance = velocity * static_cast<FLOAT>(delta);
}

void GameObject::SweptAABB(
	RECTF movingObject, 
	RECTF staticObject, 
	D3DXVECTOR3 distance, //movingObject's
	D3DXVECTOR3& normal, 
	float& t) 
{
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

	if (box.right <= staticObject.left || 
		box.left >= staticObject.right || 
		box.bottom <= staticObject.top || 
		box.top >= staticObject.bottom) 
	{
		return;
	}

	if (distance.x == 0.0f && distance.y == 0.0f) {
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

	staticObject = object->GetBoundingBox();
	D3DXVECTOR3 staticVelocity = object->GetVelocity();
	D3DXVECTOR3 staticDistance = staticVelocity * static_cast<FLOAT>(delta);

	D3DXVECTOR3 relativeDistance = this->distance - staticDistance;

	movingObject = this->GetBoundingBox();	

	SweptAABB(movingObject, staticObject, relativeDistance, normal, t);

	//one-way platforms
	if (object->GetObjectID() == 205) {
		normal.x = 0.0f;
	}

	CollisionEvent* event = new CollisionEvent(t, normal, relativeDistance, object);
	return event;
}

void GameObject::CalcPotentialCollision(
	std::vector<GameObject*>* collidableObjects, 
	std::vector<LPCOLLISIONEVENT>& collisionEvents) 
{
	for (unsigned int i = 0; i < collidableObjects->size(); ++i) {
		LPCOLLISIONEVENT event = SweptAABBEx(collidableObjects->at(i));
		if (event->time >= 0.0f && event->time <= 1.0f) {
			collisionEvents.push_back(event);
		}
		else {
			delete event;
		}
	}

	std::sort(collisionEvents.begin(), collisionEvents.end(), CollisionEvent::CompareCollisionEvent);
}

//What the hell is this
void GameObject::FilterCollision(
	std::vector<LPCOLLISIONEVENT>& collisionEvents, 
	std::vector<LPCOLLISIONEVENT>& eventResults, 
	D3DXVECTOR2& minTime, 
	D3DXVECTOR3& normal, 
	D3DXVECTOR3& relativeDistance) 
{
	minTime = D3DXVECTOR2(1.0f, 1.0f);
	normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	int minIndX = -1, minIndY = -1;

	eventResults.clear();

	for (unsigned int i = 0; i < collisionEvents.size(); ++i) {
		LPCOLLISIONEVENT coEvent = collisionEvents.at(i);

		if (coEvent->time < minTime.x && coEvent->normal.x != 0) {
			minTime.x = coEvent->time;
			normal.x = coEvent->normal.x;
			relativeDistance.x = coEvent->distance.x;
			minIndX = i;
		}

		if (coEvent->time < minTime.y && coEvent->normal.y != 0) {
			minTime.y = coEvent->time;
			normal.y = coEvent->normal.y;
			relativeDistance.y = coEvent->distance.y;
			minIndY = i;
		}
	}

	if (minIndX >= 0) {
		eventResults.push_back(collisionEvents.at(minIndX));
	}

	if (minIndY >= 0) {
		eventResults.push_back(collisionEvents.at(minIndY));
	}
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
#include "GameObject.h"

GameObject::GameObject() {
	_normal = D3DXVECTOR2(1.0f, 1.0f);
	_scale = D3DXVECTOR2(1.0f, 1.0f);
}

GameObject::~GameObject() {}

void GameObject::SetOjectType(GameObjectType objectType) {
	_objectType = objectType;
}

GameObject::GameObjectType GameObject::GetObjectType() const
{
	return _objectType;
}

RECTF GameObject::GetBoundingBox(int index) const {
	RECTF bound = _hitbox.GetBoundingBox(index);
	bound.left = _position.x;
	bound.top = _position.y;
	bound.right = _position.x + _hitbox.GetBoxWidth(index);
	bound.bottom = _position.y + _hitbox.GetBoxHeight(index);
	return bound;
}

float GameObject::GetBoxWidth(int index) const {
	return _hitbox.GetBoxWidth(index);
}

float GameObject::GetBoxHeight(int index) const {
	return _hitbox.GetBoxHeight(index);
}

void GameObject::SetVelocity(D3DXVECTOR2 velocity) {
	_velocity = velocity;
}

D3DXVECTOR2 GameObject::GetVelocity() const {
	return _velocity;
}

void GameObject::SetDistance(D3DXVECTOR2 distance) {
	_distance = distance;
}

D3DXVECTOR2 GameObject::GetDistance() const {
	return _distance;
}

void GameObject::SetNormal(D3DXVECTOR2 normal) {
	_normal = normal;
}

D3DXVECTOR2 GameObject::GetNormal() const {
	return _normal;
}

void GameObject::SetPosition(D3DXVECTOR2 position) {
	_position = position;
}

D3DXVECTOR2 GameObject::GetPosition() const {
	return _position;
}

void GameObject::SetRotation(D3DXVECTOR2 rotation) {
	_rotation = rotation;
}

D3DXVECTOR2 GameObject::GetRotation() const {
	return _rotation;
}

void GameObject::SetTranslation(D3DXVECTOR2 translation) {
	_translation = translation;
}

D3DXVECTOR2 GameObject::GetTranslation() const {
	return _translation;
}

void GameObject::SetScale(D3DXVECTOR2 scale) {
	_scale = scale;
}

D3DXVECTOR2 GameObject::GetScale() const {
	return _scale;
}

void GameObject::SweptAABB(
	RECTF movingObject, 
	RECTF staticObject, 
	D3DXVECTOR2 distance, 
	D3DXVECTOR2& normal, 
	float& time) 
{
	D3DXVECTOR2 dEntry;
	D3DXVECTOR2 dExit;

	D3DXVECTOR2 tEntry;
	D3DXVECTOR2 tExit;

	float entry;
	float exit;

	time = -1.0f;
	normal = D3DXVECTOR2(0, 0);

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

	time = entry;

	if (tEntry.x > tEntry.y) {
		normal.y = 0.0f;
		distance.x > 0 ? normal.x = -1.0f : normal.x = 1.0f;
	}
	else {
		normal.x = 0.0f;
		distance.y > 0 ? normal.y = -1.0f : normal.y = 1.0f;
	}
}

CollisionEvent* GameObject::SweptAABBEx(GameObject*& object) {
	RECTF movingObject;
	RECTF staticObject;
	D3DXVECTOR2 normal;
	float time;

	staticObject = object->GetBoundingBox();
	D3DXVECTOR2 staticVeloctity = object->GetVelocity();
	D3DXVECTOR2 staticDistance = staticVeloctity * static_cast<float>(_deltaTime);
	D3DXVECTOR2 relativeDistance = _distance - staticDistance;

	movingObject = this->GetBoundingBox();
	SweptAABB(movingObject, staticObject, relativeDistance, normal, time);
	
	return new CollisionEvent(object, normal, relativeDistance, time);
}



void GameObject::CalcPotentialCollision(std::vector<GameObject*>* collidableObjects, std::vector<LPCOLLISIONEVENT>& collisionEvents) {
	for (unsigned int i = 0; i < collidableObjects->size(); ++i) {
		LPCOLLISIONEVENT event = SweptAABBEx(collidableObjects->at(i));
		if (event->time >= 0.0f && event->time <= 1.0f) {
			collisionEvents.push_back(event);
		}
		else {
			delete event;
			event = nullptr;
		}
	}

	std::sort(collisionEvents.begin(), collisionEvents.end(), CollisionEvent::CompareCollisionEvent);
}

void GameObject::FilterCollision(
	const std::vector<LPCOLLISIONEVENT>& collisionEvents, 
	std::vector<LPCOLLISIONEVENT>& eventsResult, 
	D3DXVECTOR2& minTime, 
	D3DXVECTOR2& normal, 
	D3DXVECTOR2& relativeDistance) 
{
	minTime = D3DXVECTOR2(1.0f, 1.0f);
	normal = D3DXVECTOR2(0.0f, 0.0f);

	int minIndX = -1, minIndY = -1;

	eventsResult.clear();

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
		eventsResult.push_back(collisionEvents.at(minIndX));
	}

	if (minIndY >= 0) {
		eventsResult.push_back(collisionEvents.at(minIndY));
	}
}

void GameObject::Update(DWORD deltaTime, std::vector<GameObject*>* collidableObjects) {
	_deltaTime = deltaTime;
	_distance = _velocity * static_cast<float>(_deltaTime);
}

void GameObject::Render() {}
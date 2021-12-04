#include "GameObject.h"
#include "Entity.h"

GameObject::GameObject() {
	_isActive = true;
	_normal = D3DXVECTOR2(1.0f, 1.0f);
	_scale = D3DXVECTOR2(1.0f, 1.0f);
}

GameObject::~GameObject() {}

void GameObject::SetActive(bool active) {
	if (_isActive != active) {
		_isActive = active;
	}
}

bool GameObject::IsActive() const {
	return _isActive;
}

bool GameObject::IsOverlapped(GameObject* object) const {
	/*
	* boxSize		entityBoxSize
	* x,y			x,y
	* |----|		|----|
	* |    |		|    |
	* |----|		|----|
	* Overlap if half of the sum of two hitboxes is greater than the positions of the entities
	* Use paint to visualise
	* Edge to edge detection+
	*/

	D3DXVECTOR2 boxSize = { _hitbox.GetBoxWidth(), _hitbox.GetBoxHeight() };
	D3DXVECTOR2 entityBoxSize = { object->GetBoxWidth(), object->GetBoxHeight() };
	return (abs(_position.x - object->GetPosition().x) < abs((boxSize.x + entityBoxSize.x) / 2.0f)) &&
		(abs(_position.y - object->GetPosition().y) < abs((boxSize.y + entityBoxSize.y) / 2.0f));
}

void GameObject::SetOjectType(GameObjectType objectType) {
	_objectType = objectType;
}

GameObject::GameObjectType GameObject::GetObjectType() const
{
	return _objectType;
}

RECTF GameObject::GetBoundingBox(int index) const {
	RECTF bound;
	bound.left = _position.x + _hitbox.GetBoxOffsetX(index);
	bound.top = _position.y + _hitbox.GetBoxOffsetY(index);
	bound.right = bound.left + _hitbox.GetBoxWidth(index);
	bound.bottom = bound.top + _hitbox.GetBoxHeight(index);
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

	//No collision
	time = -1.0f;
	normal = D3DXVECTOR2(0, 0);

	//Broad phase test
	RECTF box;
	box.left = distance.x > 0 ? movingObject.left : (movingObject.left + distance.x);
	box.top = distance.y > 0 ? movingObject.top : (movingObject.top + distance.y);
	box.right = distance.x > 0 ? (movingObject.right + distance.x) : movingObject.right;
	box.bottom = distance.y > 0 ? (movingObject.bottom + distance.y) : movingObject.bottom;

	if (box.right < staticObject.left ||
		box.left > staticObject.right ||
		box.bottom < staticObject.top ||
		box.top > staticObject.bottom)
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

	for (unsigned int i = 0; i < collisionEvents.size(); ++i) {
		LPCOLLISIONEVENT coEvent = collisionEvents.at(i);

		if (coEvent->entity->isPassThroughable) {
			continue;
		}

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
		eventsResult.emplace_back(collisionEvents.at(minIndX));
	}

	if (minIndY >= 0) {
		eventsResult.emplace_back(collisionEvents.at(minIndY));
	}

	for (unsigned int i = 0; i < collisionEvents.size(); ++i) {
		LPCOLLISIONEVENT coEvent = collisionEvents.at(i);
		if (coEvent->entity->isPassThroughable) {
			eventsResult.emplace_back(coEvent);
		}
	}
}

void GameObject::Update(DWORD deltaTime, std::vector<GameObject*>* collidableObjects) {
	_deltaTime = deltaTime;
	_distance = _velocity * static_cast<float>(_deltaTime);
}

void GameObject::Render() {}
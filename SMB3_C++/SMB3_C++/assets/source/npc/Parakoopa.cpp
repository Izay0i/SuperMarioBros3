#include "../../headers/npc/Parakoopa.h"

Parakoopa::Parakoopa() {
	//0 - dead
	//1 - spin
	//2 - shell
	//3 - normal
	//4 - flying
	hitPoints = 4;
}

void Parakoopa::HandleStates() {
	KoopaTroopa::HandleStates();

	switch (hitPoints) {
		case 4:
			currentState = KoopaState::FLY;
			break;
	}

	switch (currentState) {
		case KoopaState::FLY:
			velocity.x = -runSpeed * normal.x;
			scale = D3DXVECTOR2(1.0f, 1.0f);
			break;
	}
}

void Parakoopa::Update(DWORD delta, std::vector<GameObject*>* objects) {
	if (hitPoints == 0 && !IsBeingRemoved()) {
		StartRemoveTimer();
	}
	
	HandleStates();

	GameObject::Update(delta);
	velocity.y += gravity * delta;

	if (removeStart != 0 && GetTickCount64() - removeStart > removeTime) {
		hitPoints = -1;
		removeStart = 0;
	}

	if (hitPoints == 4 && isOnGround) {
		velocity.y = -jumpSpeed;
		isOnGround = false;
	}

	if (retractStart != 0) {
		if (GetTickCount64() - retractStart > retractTime) {
			retractStart = 0;
			hitPoints = 3;
		}
	}

	//shell spinning
	if (hitPoints == 1) {
		StartRetract();
	}

	std::vector<LPCOLLISIONEVENT> collisionEvents, eventResults;
	collisionEvents.clear();

	if (hitPoints != 0) {
		CalcPotentialCollision(objects, collisionEvents);
	}

	if (collisionEvents.size() == 0) {
		position += distance;
	}
	else {
		D3DXVECTOR2 minTime;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 relativeDistance;

		FilterCollision(collisionEvents, eventResults, minTime, normal, relativeDistance);

		position.x += minTime.x * distance.x + normal.x * 0.4f;
		position.y += minTime.y * distance.y + normal.y * 0.4f;

		if (normal.x != 0.0f) {
			velocity.x = 0.0f;
		}

		if (normal.y != 0.0f) {
			velocity.y = 0.0f;
		}

		for (LPCOLLISIONEVENT result : eventResults) {
			LPCOLLISIONEVENT event = result;

			if (dynamic_cast<Tiles*>(event->object) ||
				dynamic_cast<QuestionBlock*>(event->object) ||
				dynamic_cast<ShinyBrick*>(event->object))
			{
				if (event->normal.y == -1.0f) {
					isOnGround = true;
				}
			}

			//mario's fireball
			if (dynamic_cast<Entity*>(event->object) && event->object->GetObjectID() == 99) {
				hitPoints = 0;
				scale = D3DXVECTOR2(1.0f, -1.0f);
				velocity.y = -0.23f;
			}

			//ditto
			if (dynamic_cast<KoopaTroopa*>(event->object) &&
				(dynamic_cast<KoopaTroopa*>(event->object)->GetCurrentHitPoints() == 1 || isBeingHeld))
			{
				KoopaTroopa* koopa = static_cast<KoopaTroopa*>(event->object);
				koopa->SetCurrenHitPoints(0);
				koopa->SetScale(D3DXVECTOR2(1.0f, -1.0f));
				koopa->SetVelocity(D3DXVECTOR3(0, -0.23f, 0));

				hitPoints = 0;
				scale = D3DXVECTOR2(1.0f, -1.0f);
				velocity.y = -0.23f;
			}

			if (dynamic_cast<Entity*>(event->object)) {
				Entity* entity = static_cast<Entity*>(event->object);
				if (hitPoints == 1) {
					if (dynamic_cast<ShinyBrick*>(event->object) && event->normal.y == 0.0f) {
						entity->SetCurrenHitPoints(-1);
					}
					else {
						entity->TakeDamage();
					}
				}
			}

			if (dynamic_cast<Entity*>(event->object) || dynamic_cast<Tiles*>(event->object)) {
				if (event->normal.x != 0.0f) {
					if (hitPoints != 1 || //spinning
						dynamic_cast<Tiles*>(event->object) ||
						dynamic_cast<ShinyBrick*>(event->object) ||
						dynamic_cast<QuestionBlock*>(event->object))
					{
						this->normal.x = -event->normal.x;
					}
				}
			}
		}
	}

	for (LPCOLLISIONEVENT event : collisionEvents) {
		delete event;
	}
}

void Parakoopa::Render() {
	switch (currentState) {
		case KoopaState::FLY:
			sprite.PlayAnimation("Fly", position, D3DXVECTOR2(normal.x, 1.0f));
			break;
		case KoopaState::WALK:
			sprite.PlayAnimation("Walk", position, D3DXVECTOR2(normal.x, 1.0f));
			break;
		case KoopaState::RETRACT:
			sprite.PlayAnimation("Retract", D3DXVECTOR3(position.x, position.y + 10, 0), scale);

			if (GetTickCount64() - retractStart > (retractTime * 3 / 4)) {
				sprite.PlayAnimation("Out", D3DXVECTOR3(position.x, position.y + 10, 0), scale);
			}
			break;
		case KoopaState::SPIN:
			sprite.PlayAnimation("Spin", D3DXVECTOR3(position.x, position.y - 1, 0), scale);
			break;
		case KoopaState::DIE:
			sprite.PlayAnimation("Retract", position, scale);
			break;
	}
}

void Parakoopa::Release() {

}
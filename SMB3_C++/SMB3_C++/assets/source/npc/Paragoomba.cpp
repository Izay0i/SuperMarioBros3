#include "../../headers/npc/Paragoomba.h"

Paragoomba::Paragoomba() {
	//0 - dead
	//1 - walk
	//2 - fly
	hitPoints = 2;
}

void Paragoomba::HandleJumping() {
	if (isOnGround && jumpCount < 4) {
		++jumpCount;
		
		if (jumpCount == 4) {
			StartWalkTimer();
		}
	}
	
	if (hitPoints == 2 && isOnGround) {
		if (!IsTired()) {
			float mod = 1.0f;
			if (jumpCount == 3) {
				mod = 1.8f;
			}

			velocity.y = -jumpSpeed * mod;
			isOnGround = false;

			/*char debugStr[100];
			sprintf_s(debugStr, "Jump speed: %f\n", velocity.y);
			OutputDebugStringA(debugStr);*/
		}
	}	
}

void Paragoomba::HandleStates() {
	Goomba::HandleStates();
	
	switch (hitPoints) {
		case 2:
			currentState = GoombaState::FLY;
			break;
	}

	switch (currentState) {
		case GoombaState::FLY:
			velocity.x = -runSpeed * normal.x;
			break;
			break;
	}
}

void Paragoomba::Update(DWORD delta, std::vector<GameObject*>* objects) {
	HandleJumping();
	HandleStates();

	GameObject::Update(delta);

	velocity.y += gravity * delta;

	if (walkStart != 0 && GetTickCount64() - walkStart > walkTime) {
		walkStart = 0;
		jumpCount = 0;
	}

	if (removeStart != 0 && GetTickCount64() - removeStart > removeTime) {
		hitPoints = -1;
		removeStart = 0;
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

			//mario's fireball
			if (dynamic_cast<Entity*>(event->object) && event->object->GetObjectID() == 99) {
				animName = "Walk";
				scale = D3DXVECTOR2(1.0f, -1.0f);
				velocity.y = -0.33f;
				hitPoints = 0;
			}

			if (dynamic_cast<Tiles*>(event->object) ||
				dynamic_cast<QuestionBlock*>(event->object) ||
				dynamic_cast<ShinyBrick*>(event->object))
			{
				if (event->normal.y == -1.0f) {
					isOnGround = true;
				}
			}

			if (dynamic_cast<Entity*>(event->object) || dynamic_cast<Tiles*>(event->object)) {
				if (event->normal.x != 0.0f) {
					this->normal.x = -event->normal.x;
				}
			}
		}
	}

	for (LPCOLLISIONEVENT event : collisionEvents) {
		delete event;
	}
}

void Paragoomba::Render() {
	Goomba::Render();

	switch (currentState) {
		case GoombaState::FLY:
			if (!isOnGround) {
				sprite.PlayAnimation("Fly", D3DXVECTOR3(position.x, position.y - 8, 0));
			}
			else {
				sprite.PlayAnimation("Idle", D3DXVECTOR3(position.x, position.y - 5, 0));
			}
			break;
	}
}

void Paragoomba::Release() {

}
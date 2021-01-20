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
	}
}

void Paragoomba::Update(DWORD delta, std::vector<GameObject*>* objects) {
	if (hitPoints == 0 && !IsBeingRemoved()) {
		StartRemoveTimer();
	}
	
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
		D3DXVECTOR2 offSet(0.4f, 0.4f);
		D3DXVECTOR3 normal;
		D3DXVECTOR3 relativeDistance;

		FilterCollision(collisionEvents, eventResults, minTime, normal, relativeDistance);	

		for (LPCOLLISIONEVENT result : eventResults) {
			LPCOLLISIONEVENT event = result;

			//isOnGround true when:
			//on tiles
			//on portals
			//on question blocks
			//on moving platforms
			//on shiny bricks if their hp != 3
			if ((dynamic_cast<Tiles*>(event->object) ||
				dynamic_cast<Portal*>(event->object) ||
				dynamic_cast<QuestionBlock*>(event->object) ||
				dynamic_cast<MovingPlatform*>(event->object)) &&
				event->normal.y == -1.0f ||
				(dynamic_cast<ShinyBrick*>(event->object) &&
					dynamic_cast<ShinyBrick*>(event->object)->GetCurrentHitPoints() != 3))
			{
				isOnGround = true;
			}

			//venus' fireball
			if (dynamic_cast<Entity*>(event->object) && event->object->GetObjectID() == 98) {
				minTime.x = 1.0f;
				offSet.x = normal.x = relativeDistance.x = 0.0f;
				if (!isOnGround) {
					minTime.y = 1.0f;
					offSet.y = normal.y = relativeDistance.y = 0.0f;
				}
			}

			//mushroom
			if (dynamic_cast<SuperMushroom*>(event->object)) {
				minTime.x = 1.0f;
				offSet.x = normal.x = relativeDistance.x = 0.0f;
				if (!isOnGround) {
					minTime.y = 1.0f;
					offSet.y = normal.y = relativeDistance.y = 0.0f;
				}
			}

			//1up shroom
			if (dynamic_cast<GMushroom*>(event->object)) {
				minTime.x = 1.0f;
				offSet.x = normal.x = relativeDistance.x = 0.0f;
				if (!isOnGround) {
					minTime.y = 1.0f;
					offSet.y = normal.y = relativeDistance.y = 0.0f;
				}
			}

			//leaf
			if (dynamic_cast<SuperLeaf*>(event->object)) {
				minTime.x = 1.0f;
				offSet.x = normal.x = relativeDistance.x = 0.0f;
				if (!isOnGround) {
					minTime.y = 1.0f;
					offSet.y = normal.y = relativeDistance.y = 0.0f;
				}
			}

			//coin
			if (dynamic_cast<Coin*>(event->object) && dynamic_cast<Coin*>(event->object)->GetCurrentHitPoints() != 3) {
				minTime.x = 1.0f;
				offSet.x = normal.x = relativeDistance.x = 0.0f;
				if (!isOnGround) {
					minTime.y = 1.0f;
					offSet.y = normal.y = relativeDistance.y = 0.0f;
				}
			}

			//shiny brick is coin
			if (dynamic_cast<ShinyBrick*>(event->object) && dynamic_cast<ShinyBrick*>(event->object)->GetCurrentHitPoints() == 3) {
				minTime.x = 1.0f;
				offSet.x = normal.x = relativeDistance.x = 0.0f;
				if (!isOnGround) {
					minTime.y = 1.0f;
					offSet.y = normal.y = relativeDistance.y = 0.0f;
				}
			}

			//goomba
			if (dynamic_cast<Entity*>(event->object) && event->object->GetObjectID() == 1) {
				minTime.x = 1.0f;
				offSet.x = normal.x = relativeDistance.x = 0.0f;
				if (!isOnGround) {
					minTime.y = 1.0f;
					offSet.y = normal.y = relativeDistance.y = 0.0f;
				}
			}

			//shell is held
			if ((event->object->GetObjectID() == 3 || event->object->GetObjectID() == 4) && dynamic_cast<Entity*>(event->object)->IsBeingHeld()) {
				dynamic_cast<Entity*>(event->object)->SetCurrenHitPoints(0);
				event->object->SetScale(D3DXVECTOR2(1.0f, -1.0f));
				event->object->SetVelocity(D3DXVECTOR3(0, -0.23f, 0));

				animName = "Walk";
				scale = D3DXVECTOR2(1.0f, -1.0f);
				velocity.y = -0.23f;
				hitPoints = 0;
			}

			//mario's fireball or koopa shell
			if (dynamic_cast<Entity*>(event->object) && event->object->GetObjectID() == 99 ||
				((event->object->GetObjectID() == 3 || event->object->GetObjectID() == 4) && dynamic_cast<Entity*>(event->object)->GetCurrentHitPoints() == 1))
			{
				animName = "Walk";
				scale = D3DXVECTOR2(1.0f, -1.0f);
				velocity.y = -0.23f;
				hitPoints = 0;
			}

			//switch side when collide with anything except
			//mushroom 8
			//1up shroom 9
			//leaf 10
			//coin 101
			if ((dynamic_cast<Entity*>(event->object) &&
				event->object->GetObjectID() != 8 &&
				event->object->GetObjectID() != 9 &&
				event->object->GetObjectID() != 10 &&
				event->object->GetObjectID() != 101)
				|| dynamic_cast<Tiles*>(event->object)) {
				if (event->normal.x != 0.0f) {
					this->normal.x = -event->normal.x;
				}
			}
		}

		if (normal.x != 0.0f) {
			velocity.x = 0.0f;
		}

		if (normal.y != 0.0f) {
			velocity.y = 0.0f;
		}

		position.x += minTime.x * distance.x + normal.x * offSet.x;
		position.y += minTime.y * distance.y + normal.y * offSet.y;
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

	if (hitPoints == -1) {
		sprite.PlayAnimation("100", position);
	}
}

void Paragoomba::Release() {

}
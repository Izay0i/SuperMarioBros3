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
			if (extraData.size() > 0 && extraData.at(0) == "g") {
				velocity.x = -runSpeed * normal.x;
			}
			scale = D3DXVECTOR2(1.0f, 1.0f);
			break;
	}
}

void Parakoopa::Update(DWORD delta, std::vector<GameObject*>* objects) {
	if (hitPoints == 0 && !IsBeingRemoved()) {
		StartRemoveTimer();
	}
	
	HandleStates();

	if (extraData.size() > 0 && extraData.at(0) == "g") {
		if (hitPoints == 4 && isOnGround) {
			velocity.y = -jumpSpeed;
			isOnGround = false;
		}
	}
	else {
		if (hitPoints == 4) {
			if (position.y <= originalPos.y) {
				velocity.y = 0.05f;
			}
			else if (position.y >= originalPos.y + MIN_Y_OFFSET) {
				velocity.y = -0.05f;
			}
		}
	}

	GameObject::Update(delta);
	if (extraData.size() > 0 && extraData.at(0) == "g" || hitPoints != 4) {
		velocity.y += gravity * delta;
	}

	if (resetScoreStart != 0 && GetTickCount64() - resetScoreStart > resetScoreTime) {
		multiplier = 1;
		resetScoreStart = 0;
	}

	if (removeStart != 0 && GetTickCount64() - removeStart > removeTime) {
		hitPoints = -1;
		removeStart = 0;
	}

	if (retractStart != 0 && GetTickCount64() - retractStart > retractTime) {
		retractStart = 0;
		hitPoints = 3;
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

			//death
			if (dynamic_cast<Tiles*>(event->object) && event->object->GetObjectID() == 666) {
				position.y = 1000;
				StartRemoveTimer();
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

			//mario's fireball
			if (dynamic_cast<Entity*>(event->object) && event->object->GetObjectID() == 99) {
				tookDamage = true;
				hitPoints = 0;
				scale = D3DXVECTOR2(1.0f, -1.0f);
				velocity.y = -0.23f;
			}

			//koopa
			if (dynamic_cast<Entity*>(event->object) && event->object->GetObjectID() == 3) {
				minTime.x = 1.0f;
				offSet.x = normal.x = relativeDistance.x = 0.0f;
				if (!isOnGround) {
					minTime.y = 1.0f;
					offSet.y = normal.y = relativeDistance.y = 0.0f;
				}
			}

			//ditto
			if (dynamic_cast<KoopaTroopa*>(event->object) &&
				(dynamic_cast<KoopaTroopa*>(event->object)->GetCurrentHitPoints() == 1 || isBeingHeld))
			{
				KoopaTroopa* koopa = static_cast<KoopaTroopa*>(event->object);
				koopa->SetCurrenHitPoints(0);
				koopa->SetScale(D3DXVECTOR2(1.0f, -1.0f));
				koopa->SetVelocity(D3DXVECTOR3(0, -0.23f, 0));

				tookDamage = true;
				hitPoints = 0;
				scale = D3DXVECTOR2(1.0f, -1.0f);
				velocity.y = -0.23f;
			}

			//deal damage to other entities
			if (dynamic_cast<Entity*>(event->object)) {
				Entity* entity = static_cast<Entity*>(event->object);
				if (hitPoints == 1) {
					if (dynamic_cast<ShinyBrick*>(event->object) && entity->GetCurrentHitPoints() != 3 && event->normal.y == 0.0f) {
						//entity->SetCurrenHitPoints(-1);
						entity->StartRemoveTimer();
					}
					else {
						//deal damage to anything that's not:
						//mushroom 8
						//1up shroom 9
						//leaf 10
						//venus' fireball 98
						//coin 101
						//bonus item 104
						//moving platform 109
						if (event->object->GetObjectID() != 8 &&
							event->object->GetObjectID() != 9 &&
							event->object->GetObjectID() != 10 &&
							event->object->GetObjectID() != 98 &&
							event->object->GetObjectID() != 101 &&
							event->object->GetObjectID() != 104 &&
							event->object->GetObjectID() != 109)
						{
							entity->TakeDamage();
						}
					}
				}
			}

			//switch side when collide with anything except
			//mushroom 8
			//1up shroom 9
			//leaf 10
			//coin 101
			//when brick turns to coin 103
			//koopa 3
			if ((dynamic_cast<Entity*>(event->object) &&
				event->object->GetObjectID() != 3 &&
				event->object->GetObjectID() != 8 &&
				event->object->GetObjectID() != 9 &&
				event->object->GetObjectID() != 10 &&
				event->object->GetObjectID() != 101) ||
				(dynamic_cast<ShinyBrick*>(event->object) && dynamic_cast<Entity*>(event->object)->GetCurrentHitPoints() != 3) ||
				dynamic_cast<Tiles*>(event->object)) {
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

		position.x += minTime.x * distance.x + normal.x * 0.4f;
		position.y += minTime.y * distance.y + normal.y * 0.4f;
	}

	for (LPCOLLISIONEVENT event : collisionEvents) {
		delete event;
	}
}

void Parakoopa::Render() {
	KoopaTroopa::Render();

	switch (currentState) {
		case KoopaState::FLY:
			if (extraData.size() > 0 && extraData.at(0) == "g") {
				sprite.PlayAnimation("FlyGreen", position, D3DXVECTOR2(normal.x, 1.0f));
			}
			else {
				sprite.PlayAnimation("FlyRed", position, D3DXVECTOR2(normal.x, 1.0f));
			}
			break;
	}
}

void Parakoopa::Release() {

}
#include "../../headers/npc/VenusFire.h"

VenusFire::VenusFire() {
	scale = D3DXVECTOR2(1, 1);
	normal = D3DXVECTOR3(1, 1, 0);
}

Fireball* VenusFire::SpawnFireball() {
	Fireball* fireball = new Fireball;
	fireball->SetObjectID(98);

	int ind = 0;
	if (extraData.at(0) == "g") {
		ind = 1;
	}

	fireball->ParseData(extraData.at(ind), extraData.at(ind + 1), colorKey);
	fireball->SetNormal(D3DXVECTOR3(-normal.x, normal.y, 0));
	fireball->SetPosition(D3DXVECTOR3(position.x + normal.x, position.y + 10, 0));
	return fireball;
}

void VenusFire::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::vector<std::string> extraData) {
	PiranaPlant::ParseData(dataPath, texturePath, colorKey, extraData);

	if (extraData.at(0) == "g") {
		MAX_Y_OFFSET = 40.0f;
	}
}

void VenusFire::Update(DWORD delta, std::vector<GameObject*>* objects) {	
	PiranaPlant::HandleStates();

	if (IsRetracting()) {
		if (position.y > (originalPos.y - MAX_Y_OFFSET)) {
			position.y -= 0.02f * delta;
		}

		if (retractStart != 0 && GetTickCount64() - retractStart == (retractTime * 3 / 4) && !playerInRange) {
			SceneManager::GetInstance()->GetCurrentScene()->AddObjectToScene(SpawnFireball());
		}
	}
	
	if (!IsRetracting() || playerInRange) {
		if (position.y >= originalPos.y) {
			position.y = originalPos.y;
			StartRetractTimer();
		}
		else {
			position.y += 0.02f * delta;
		}
	}

	if (retractStart != 0 && GetTickCount64() - retractStart > retractTime) {
		retractStart = 0;
	}

	if (resetScoreStart != 0 && GetTickCount64() - resetScoreStart > resetScoreTime) {
		multiplier = 1;
		resetScoreStart = 0;
	}

	if (removeStart != 0 && GetTickCount64() - removeStart > removeTime) {
		hitPoints = -1;
		removeStart = 0;
	}

	scale.x = normal.x == 1 ? 1.0f : -1.0f;

	std::vector<LPCOLLISIONEVENT> collisionEvents, eventResults;
	collisionEvents.clear();

	CalcPotentialCollision(objects, collisionEvents);

	D3DXVECTOR2 minTime;
	D3DXVECTOR2 offSet(0.4f, 0.4f);
	D3DXVECTOR3 normal;
	D3DXVECTOR3 relativeDistance;

	FilterCollision(collisionEvents, eventResults, minTime, normal, relativeDistance);

	for (LPCOLLISIONEVENT result : eventResults) {
		LPCOLLISIONEVENT event = result;

		//mario's fireball
		if (dynamic_cast<Entity*>(event->object) && event->object->GetObjectID() == 99) {
			tookDamage = true;
			StartRemoveTimer();
		}
	}

	for (LPCOLLISIONEVENT event : collisionEvents) {
		delete event;
	}
}

void VenusFire::Render() {
	switch (currentState) {
		case PlantState::BITING:
			//up
			if (normal.y == 1) {
				if (extraData.at(0) == "g") {
					if (GetTickCount64() - retractStart > (retractTime * 3 / 5)) {
						sprite.PlayAnimation("LookUpOpenGreen", position, scale);
					}
					else {
						sprite.PlayAnimation("LookUpGreen", position, scale);
					}
				}
				else {
					if (GetTickCount64() - retractStart > (retractTime * 3 / 5)) {
						sprite.PlayAnimation("LookUpOpenRed", position, scale);
					}
					else {
						sprite.PlayAnimation("LookUpRed", position, scale);
					}
				}
			}
			//down
			else if (normal.y == -1) {
				if (extraData.at(0) == "g") {
					if (GetTickCount64() - retractStart > (retractTime * 3 / 5)) {
						sprite.PlayAnimation("LookDownOpenGreen", position, scale);
					}
					else {
						sprite.PlayAnimation("LookDownGreen", position, scale);
					}
				}
				else {
					if (GetTickCount64() - retractStart > (retractTime * 3 / 5)) {
						sprite.PlayAnimation("LookDownOpenRed", position, scale);
					}
					else {
						sprite.PlayAnimation("LookDownRed", position, scale);
					}
				}
			}
			break;
		case PlantState::DIE:
			sprite.PlayAnimation("Die", position, scale);
			break;
	}

	if (resetScoreStart != 0 && GetTickCount64() - resetScoreStart > resetScoreTime / 4) {
		sprite.PlayAnimation(ScoreToString(score), D3DXVECTOR3(position.x + 1, position.y - 16, 0));
	}
}

void VenusFire::Release() {

}
#include "../../headers/npc/VenusFire.h"

VenusFire::VenusFire() {
	scale = D3DXVECTOR2(1, 1);
	normal = D3DXVECTOR3(1, 1, 0);
}

RECTF VenusFire::GetBoundingBox(int id) const {
	RECTF bound;

	if (hitPoints > 0) {
		bound.left = position.x;
		bound.top = position.y;
		bound.right = position.x + hitBox.GetWidth(id);
		bound.bottom = position.y + hitBox.GetHeight(id);
	}

	return bound;
}

Fireball* VenusFire::SpawnFireball() {
	Fireball* fireball = new Fireball;
	fireball->SetObjectID(98);

	int ind = 0;
	if (extraData.at(0) == "g") {
		ind = 1;
	}

	fireball->ParseData(extraData.at(ind), extraData.at(ind + 1), colorKey);
	fireball->SetNormal(D3DXVECTOR3(-normal.x, 0, 0));
	fireball->SetPosition(D3DXVECTOR3(position.x, position.y + 10, 0));
	return fireball;
}

void VenusFire::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::vector<std::string> extraData) {
	PiranaPlant::ParseData(dataPath, texturePath, colorKey, extraData);

	if (extraData.at(0) == "g") {
		MAX_Y_OFFSET = 40.0f;
	}
}

void VenusFire::Update(DWORD delta, std::vector<GameObject*>* objects) {	
	if (IsRetracting()) {
		if (position.y > (originalPos.y - MAX_Y_OFFSET)) {			
			position.y -= 0.5f;
		}

		if (GetTickCount64() - retractStart == (retractTime * 3 / 4)) {
			SceneManager::GetInstance()->GetCurrentScene()->AddObjectToScene(SpawnFireball());
		}
	}

	if (!IsRetracting()) {
		if (position.y >= originalPos.y) {
			position.y = originalPos.y;
			StartRetractTimer();
		}
		else {
			position.y += 0.5f;
		}
	}

	if (retractStart != 0 && GetTickCount64() - retractStart > retractTime) {
		retractStart = 0;
	}

	if (removeStart != 0 && GetTickCount64() - removeStart > removeTime) {
		hitPoints = -1;
		removeStart = 0;
	}

	scale.x = normal.x == 1 ? 1.0f : -1.0f;
}

void VenusFire::Render() {
	switch (currentState) {
		case PlantState::BITING:
			//up
			if (normal.y == 1) {
				if (extraData.at(0) == "g") {
					if (GetTickCount64() - retractStart > (retractTime * 3 / 4)) {
						sprite.PlayAnimation("LookUpOpenGreen", position, scale);
					}
					else {
						sprite.PlayAnimation("LookUpGreen", position, scale);
					}
				}
				else {
					if (GetTickCount64() - retractStart > (retractTime * 3 / 4)) {
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
					if (GetTickCount64() - retractStart > (retractTime * 3 / 4)) {
						sprite.PlayAnimation("LookDownOpenGreen", position, scale);
					}
					else {
						sprite.PlayAnimation("LookDownGreen", position, scale);
					}
				}
				else {
					if (GetTickCount64() - retractStart > (retractTime * 3 / 4)) {
						sprite.PlayAnimation("LookDownOpenRed", position, scale);
					}
					else {
						sprite.PlayAnimation("LookDownRed", position, scale);
					}
				}
			}
			break;
	}
}

void VenusFire::Release() {

}
#include "../../headers/npc/VenusFire.h"

VenusFire::VenusFire() {
	scale = D3DXVECTOR2(1, 1);
	normal = D3DXVECTOR3(1, 1, 0);
}

void VenusFire::Update(DWORD delta, std::vector<GameObject*>* objects) {
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
					sprite.PlayAnimation("LookUpGreen", position, scale);
				}
				else {
					sprite.PlayAnimation("LookUpRed", position, scale);
				}
			}
			//down
			else if (normal.y == -1) {
				if (extraData.at(0) == "g") {
					sprite.PlayAnimation("LookDownGreen", position, scale);
				}
				else {
					sprite.PlayAnimation("LookDownRed", position, scale);
				}
			}
			break;
	}
}

void VenusFire::Release() {

}
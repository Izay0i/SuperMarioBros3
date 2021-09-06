#include "../SceneManager.h"
#include "../Entity.h"
#include "PiranaPlant.h"
#include "VenusPlant.h"

VenusPlant::VenusPlant() {
	_coolDownTime = 2000;
}

VenusPlant::~VenusPlant() {}

void VenusPlant::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (_position.y < _originalPos.y - _MAX_Y_OFFSET) {
		if (IsOnCoolDown() && GetTickCount64() - _coolDownStart > _coolDownTime * 0.75f) {
			//SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(SpawnFireball());
		}
	}

	PiranaPlant::Update(deltaTime, collidableEntities, collidableTiles, grid);
}

void VenusPlant::Render() {
	if (!_isActive) {
		return;
	}

	switch (_state) {
		case _State::BITE:
			//Looking up
			if (_normal.x == 1.0f) {
				if (IsOnCoolDown() && GetTickCount64() - _coolDownStart > _coolDownTime * 0.6f) {
					_animatedSprite.PlaySpriteAnimation(_variant == "red" ? "RedOpenUp" : "GreenOpenUp", _position, _scale);
				}
				else {
					_animatedSprite.PlaySpriteAnimation(_variant == "red" ? "RedChompUp" : "GreenChompUp", _position, _scale);
				}
			}
			//Looking down
			else if (_normal.x == -1.0f) {
				if (IsOnCoolDown() && GetTickCount64() - _coolDownStart > _coolDownTime * 0.6f) {
					_animatedSprite.PlaySpriteAnimation(_variant == "red" ? "RedOpenDown" : "GreenOpenDown", _position, _scale);
				}
				else {
					_animatedSprite.PlaySpriteAnimation(_variant == "red" ? "RedChompDown" : "GreenChompDown", _position, _scale);
				}
			}
			break;
		case _State::DIE:

			break;
	}
}
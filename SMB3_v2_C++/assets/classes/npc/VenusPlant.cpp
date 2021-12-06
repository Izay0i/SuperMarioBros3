#include "../SceneManager.h"
#include "../Entity.h"
#include "PiranaPlant.h"
#include "VenusPlant.h"
#include "../projectile/Fireball.h"

VenusPlant::VenusPlant() {
	_coolDownTime = 2000;
}

VenusPlant::~VenusPlant() {}

Fireball* VenusPlant::SpawnFireball() {
	//This is madness
	Fireball* fireball = dynamic_cast<Fireball*>(
		SceneManager::GetInstance()->GetCurrentScene()->CreateEntityFromData(
			_extraData.at(0), 
			_extraData.at(1), 
			_extraData.at(2)
		)
	);
	const float OFFSET_Y = 3.0f;
	fireball->SetNormal({ -_scale.x, _normal.x });
	fireball->SetPosition({ _position.x + _normal.x, _position.y + OFFSET_Y });
	return fireball;
}

void VenusPlant::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (_position.y <= _originalPos.y - _MAX_Y_OFFSET) {
		if (IsOnCoolDown() && GetTickCount64() - _coolDownStart == _coolDownTime * 0.75f) {
			SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(SpawnFireball());
		}
	}

	PiranaPlant::Update(deltaTime, collidableEntities, collidableTiles, grid);
}

void VenusPlant::Render() {
	switch (_state) {
		case _State::BITE:
			//Looking up
			if (_normal.x == 1.0f) {
				if (IsOnCoolDown() && GetTickCount64() - _coolDownStart > _coolDownTime * 0.6f) {
					_animatedSprite.PlaySpriteAnimation("OpenUp", _position, _scale);
				}
				else {
					_animatedSprite.PlaySpriteAnimation("ChompUp", _position, _scale);
				}
			}
			//Looking down
			else if (_normal.x == -1.0f) {
				if (IsOnCoolDown() && GetTickCount64() - _coolDownStart > _coolDownTime * 0.6f) {
					_animatedSprite.PlaySpriteAnimation("OpenDown", _position, _scale);
				}
				else {
					_animatedSprite.PlaySpriteAnimation("ChompDown", _position, _scale);
				}
			}
			break;
		case _State::DIE:
			_animatedSprite.PlaySpriteAnimation("Explode", _position);
			break;
	}
}
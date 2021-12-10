#include "../SceneManager.h"
#include "../Entity.h"
#include "../item/Orb.h"
#include "../effect/OrbEffect.h"
#include "FortressBoss.h"
#include "state/BossIdleState.h"
#include "../audio/AudioService.h"

Texture* FortressBoss::_bossTexture = nullptr;

void FortressBoss::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _bossTexture);
}

FortressBoss::FortressBoss() {
	_normal.x = -1.0f;

	_renderPriority = 1;
	_health = 3;
	_removeTime = 3200;

	_runSpeed = 0.08f;
	_jumpSpeed = 0.6f;
	_gravity = 0.002f;

	_introTime = 300;
	_attackTime = 1200;
	_coolDownTime = 5000;
	_invulnerableTime = 2000;

	_bossState = new BossIdleState(this);
}

FortressBoss::~FortressBoss() {}

bool FortressBoss::IsInIntro() const {
	return _introStart != 0;
}

void FortressBoss::StartIntroTimer() {
	_introStart = static_cast<DWORD>(GetTickCount64());
}

bool FortressBoss::IsAttacking() const {
	return _attackStart != 0;
}

void FortressBoss::StartAttackTimer() {
	_attackStart = static_cast<DWORD>(GetTickCount64());
}

bool FortressBoss::IsOnCoolDown() const {
	return _coolDownStart != 0;
}

void FortressBoss::StartCoolDownTimer() {
	_coolDownStart = static_cast<DWORD>(GetTickCount64());
}

bool FortressBoss::IsInvulnerable() const {
	return _invulnerableStart != 0;
}

void FortressBoss::StartInvulnerableTimer() {
	_invulnerableStart = static_cast<DWORD>(GetTickCount64());
}

RECTF FortressBoss::GetBoundingBox(int) const {
	return _health <= 0 ? RECTF() : GameObject::GetBoundingBox();
}

Orb* FortressBoss::SpawnOrb() {
	Orb* orb = dynamic_cast<Orb*>(
		SceneManager::GetInstance()->GetCurrentScene()->CreateEntityFromData(
			_extraData.at(0),
			_extraData.at(1),
			_extraData.at(2)
		)
	);
	orb->SetPosition({ _position.x, _position.y - GameObject::GetBoxHeight() });
	return orb;
}

OrbEffect* FortressBoss::SpawnOrbEffect() {
	OrbEffect* orbEffect = dynamic_cast<OrbEffect*>(
		SceneManager::GetInstance()->GetCurrentScene()->CreateEntityFromData(
			_extraData.at(3),
			_extraData.at(4),
			_extraData.at(5)
		)
	);
	orbEffect->SetPosition(_position);
	orbEffect->StartRemoveTimer();
	return orbEffect;
}

void FortressBoss::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_bossTexture == nullptr) {
		_bossTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void FortressBoss::TakeDamage() {
	if (_health == 0 && !IsRemoved()) {
		StartRemoveTimer();
	}

	if (commenceBattle && !IsInvulnerable()) {
		StartInvulnerableTimer();

		--_health;

		tookDamage = true;
	}
}

void FortressBoss::HandleStates() {
	isPassThroughable = IsInvulnerable();

	BossState* currentState = _bossState->HandleStates();
	if (currentState != nullptr) {
		delete _bossState;
		_bossState = currentState;
	}
}

void FortressBoss::HandleCollisionResult(
	LPCOLLISIONEVENT result, 
	D3DXVECTOR2& minTime, 
	D3DXVECTOR2& offset, 
	D3DXVECTOR2& normal, 
	D3DXVECTOR2& relativeDistance) 
{
	Entity* eventEntity = result->entity;
	D3DXVECTOR2 eventNormal = result->normal;

	switch (eventEntity->GetObjectType()) {
		case GameObjectType::GAMEOBJECT_TYPE_TILE:
			if (eventNormal.x != 0.0f) {
				_normal.x = -_normal.x;
			}

			if (eventNormal.y == -1.0f) {
				_isOnGround = true;
			}
			break;
	}
}

void FortressBoss::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (IsInIntro() && GetTickCount64() - _introStart > _introTime) {
		_introStart = 0;
	}

	if (IsAttacking() && GetTickCount64() - _attackStart > _attackTime) {
		_attackStart = 0;
		StartCoolDownTimer();
	}

	if (IsOnCoolDown() && GetTickCount64() - _coolDownStart > _coolDownTime) {
		_coolDownStart = 0;
		StartAttackTimer();
	}

	if (IsInvulnerable() && GetTickCount64() - _invulnerableStart > _invulnerableTime) {
		if (_health <= 0) {
			AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_THWOMP);
		}
		
		_invulnerableStart = 0;
	}

	HandleStates();
	_bossState->Update(deltaTime);
	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);
}

void FortressBoss::Render() {
	if (!commenceBattle) {
		_animatedSprite.PlaySpriteAnimation("Idle", _position);
	}
	else {
		_bossState->Render();
	}
}

void FortressBoss::Release() {
	if (_bossState != nullptr) {
		_bossState->Release();
		delete _bossState;
	}

	_animatedSprite.Release();
	_bossTexture = nullptr;
}
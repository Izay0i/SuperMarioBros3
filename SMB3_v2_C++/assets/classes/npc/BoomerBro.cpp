#include "../SceneManager.h"
#include "../Entity.h"
#include "BoomerBro.h"

LPDIRECT3DTEXTURE9 BoomerBro::_boomerTexture = nullptr;

void BoomerBro::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _boomerTexture);
}

BoomerBro::BoomerBro() {
	_renderPriority = 1;

	_runSpeed = 0.03f;
	_gravity = 0.0002f;

	_attackTime = 3000;
	_state = _State::WALK;
}

BoomerBro::~BoomerBro() {}

RECTF BoomerBro::GetBoundingBox(int index) const {
	return _health <= 0 ? RECTF() : GameObject::GetBoundingBox();
}

void BoomerBro::SetPosition(D3DXVECTOR2 position) {
	_position = position;
	_originalPos = position;
}

bool BoomerBro::IsAttacking() const {
	return _attackStart != 0;
}

void BoomerBro::StartAttackTimer() {
	_attackStart = static_cast<DWORD>(GetTickCount64());
}

void BoomerBro::ParseData(
	std::string dataPath, 
	const LPDIRECT3DTEXTURE9& texture, 
	std::vector<std::string> extraData) 
{
	if (_boomerTexture == nullptr) {
		_boomerTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void BoomerBro::TakeDamage() {
	_health = 0;
	tookDamage = true;
	StartRemoveTimer();
}

void BoomerBro::HandleStates() {
	switch (_health) {
		case 0:
			_state = _State::DIE;
			break;
		case 1:
			_state = _State::WALK;
			break;
	}

	switch (_state) {
		case _State::WALK:
			_velocity.x = -_runSpeed * _normal.x;
			break;
		case _State::DIE:
			_velocity.x = 0.0f;
			_scale.y = -1.0f;
			break;
	}
}

void BoomerBro::HandleCollisionResult(
	LPCOLLISIONEVENT result, 
	D3DXVECTOR2& minTime, 
	D3DXVECTOR2& offset, 
	D3DXVECTOR2& normal, 
	D3DXVECTOR2& relativeDistance) 
{

}

void BoomerBro::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	const float MAX_X_OFFSET = 8.0f;
	const float MIN_X_OFFSET = 8.0f;
	if (_position.x <= _originalPos.x - MIN_X_OFFSET) {
		_normal.x = -1.0f;
	}
	else if (_position.x >= _originalPos.x + MAX_X_OFFSET) {
		_normal.x = 1.0f;
	}

	if (!IsAttacking()) {
		//SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(SpawnBoomerang());
		StartAttackTimer();
	}

	if (IsAttacking() && GetTickCount64() - _attackStart > _attackTime) {
		_attackStart = 0;
	}

	HandleStates();
	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);
}

void BoomerBro::Render() {
	switch (_state) {
		case _State::WALK:
		case _State::DIE:
			_animatedSprite.PlaySpriteAnimation("Walk", _position, _scale);
			break;
	}
}

void BoomerBro::Release() {
	_boomerTexture = nullptr;
}

#include "../Entity.h"
#include "PiranaPlant.h"

Texture* PiranaPlant::_piranaTexture = nullptr;

void PiranaPlant::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _piranaTexture);
}

PiranaPlant::PiranaPlant() {
	_renderPriority = 2;
	_runSpeed = 0.02f;
	_gravity = 0.0f;
	
	_coolDownTime = 500;
	_state = _State::BITE;
}

PiranaPlant::~PiranaPlant() {}

bool PiranaPlant::IsOnCoolDown() const {
	return _coolDownStart != 0;
}

void PiranaPlant::StartCoolDownTimer() {
	_coolDownStart = static_cast<DWORD>(GetTickCount64());
}

RECTF PiranaPlant::GetBoundingBox(int index) const {
	return _health <= 0 ? RECTF() : GameObject::GetBoundingBox();
}

void PiranaPlant::SetPosition(D3DXVECTOR2 position) {
	_position = position;
	_originalPos = position;

	//Boundary/proximity of the plant to detect the player
	const RECTF BOUNDARY_OFFSET(1.5f, 4.0f, 2.0f, 4.0f);
	_boundary.left = _originalPos.x - _hitbox.GetBoxWidth() * BOUNDARY_OFFSET.left;
	_boundary.top = _originalPos.y - _hitbox.GetBoxHeight() * BOUNDARY_OFFSET.top;
	_boundary.right = _originalPos.x + _hitbox.GetBoxWidth() * BOUNDARY_OFFSET.right;
	_boundary.bottom = _originalPos.y + _hitbox.GetBoxHeight() * BOUNDARY_OFFSET.bottom;
}

void PiranaPlant::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_piranaTexture == nullptr) {
		_piranaTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void PiranaPlant::TakeDamage() {
	_health = 0;
	tookDamage = true;
	StartRemoveTimer();
}

void PiranaPlant::ComparePlayerPosToSelf(D3DXVECTOR2 playerPos) {
	if (playerPos.x >= _boundary.left &&
		playerPos.y >= _boundary.top &&
		playerPos.x <= _boundary.right &&
		playerPos.y <= _boundary.bottom) 
	{
		_isPlayerInRange = true;
	}
	else {
		_isPlayerInRange = false;
	}
}

void PiranaPlant::HandleStates() {
	_state = static_cast<_State>(_health);

	if (_health == 0 && !IsRemoved()) {
		StartRemoveTimer();
	}
}

void PiranaPlant::HandleCollisionResult(
	LPCOLLISIONEVENT result, 
	D3DXVECTOR2& minTime, 
	D3DXVECTOR2& offset, 
	D3DXVECTOR2& normal, 
	D3DXVECTOR2& relativeDistance) {}

void PiranaPlant::Update(
	DWORD deltaTime,
	std::vector<Entity*>* collidableEntities,
	std::vector<Entity*>* collidableTiles,
	Grid* grid)
{
	if (_isPlayerInRange) {
		if (_position.y >= _originalPos.y) {
			StartCoolDownTimer();
		}
	}

	if (!IsOnCoolDown()) {
		_position.y += _runSpeed * _normal.y * _deltaTime;

		if (_position.y < _originalPos.y - _MAX_Y_OFFSET) {
			StartCoolDownTimer();
			_position.y = _originalPos.y - _MAX_Y_OFFSET;
			_normal.y = 1.0f;
		}
		else if (_position.y > _originalPos.y) {
			StartCoolDownTimer();
			_position.y = _originalPos.y;
			_normal.y = -1.0f;
		}
	}

	if (IsOnCoolDown() && GetTickCount64() - _coolDownStart > _coolDownTime) {
		_coolDownStart = 0;
	}
	
	_position.x = _originalPos.x;
	HandleStates();
	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);
}

void PiranaPlant::Render() {
	switch (_state) {
		case _State::BITE:
			_animatedSprite.PlaySpriteAnimation("Chomp", _position);
			break;
		case _State::DIE:
			_animatedSprite.PlaySpriteAnimation("Explode", _position);
			break;
	}
}

void PiranaPlant::Release() {
	_animatedSprite.Release();
	_piranaTexture = nullptr;
}
#include "../Entity.h"
#include "PiranaPlant.h"

LPDIRECT3DTEXTURE9 PiranaPlant::_piranaTexture = nullptr;

void PiranaPlant::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _piranaTexture);
}

PiranaPlant::PiranaPlant() {
	_renderPriority = 1;
	_runSpeed = 0.02f;
	_gravity = 0.0f;

	_variant = "green";
	_coolDownStart = 3000;
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
	_boundary.left = _originalPos.x - _hitbox.GetBoxWidth() * 0.5f;
	_boundary.top = _originalPos.y - _hitbox.GetBoxHeight() * 0.5f;
	_boundary.right = _originalPos.x + _hitbox.GetBoxWidth() * 3.0f;
	_boundary.bottom = _originalPos.y + _hitbox.GetBoxHeight() * 3.0f;
}

void PiranaPlant::ParseData(std::string dataPath, const LPDIRECT3DTEXTURE9& texture, std::vector<std::string> extraData) {
	if (_piranaTexture == nullptr) {
		_piranaTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);

	if (_extraData.size() == 1) {
		_variant = _extraData.front();
	}
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
	switch (_health) {
		case 0:
			_state = _State::DIE;
			break;
		case 1:
			_state = _State::BITE;
			break;
	}
}

void PiranaPlant::HandleCollisionResult(
	LPCOLLISIONEVENT result, 
	D3DXVECTOR2& minTime, 
	D3DXVECTOR2& offset, 
	D3DXVECTOR2& normal, 
	D3DXVECTOR2& relativeDistance) 
{
	Entity* eventEntity = result->entity;
	D3DXVECTOR2 eventNormal = result->normal;

	if (eventEntity == nullptr) {
		return;
	}

	switch (eventEntity->GetObjectType()) {
		case GameObjectType::GAMEOBJECT_TYPE_PFIREBALL:
		case GameObjectType::GAMEOBJECT_TYPE_TAIL:
			TakeDamage();
			break;
	}
}

void PiranaPlant::Update(
	DWORD deltaTime,
	std::vector<Entity*>* collidableEntities,
	std::vector<Entity*>* collidableTiles,
	Grid* grid)
{
	_velocity.y = _runSpeed * _normal.y;

	const float MAX_Y_OFFSET = 38.0f;
	const float MIN_Y_OFFSET = 32.0f;
	if (_position.y > _originalPos.y + MIN_Y_OFFSET) {
		_normal.y = -1.0f;
	}
	else if (_position.y < _originalPos.y + MAX_Y_OFFSET) {
		_normal.y = 1.0f;
	}


	HandleStates();
	Entity::Update(deltaTime, collidableEntities, collidableTiles, grid);
}

void PiranaPlant::Render() {
	if (!_isActive) {
		return;
	}

	switch (_state) {
		case _State::BITE:
			_animatedSprite.PlaySpriteAnimation(_variant == "red" ? "RedChomp" : "GreenChomp", _position);
			break;
		case _State::DIE:

			break;
	}
}

void PiranaPlant::Release() {
	_piranaTexture = nullptr;
}
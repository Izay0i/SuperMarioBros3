#include "../Entity.h"
#include "PropPlayer.h"

Texture* PropPlayer::_propTexture = nullptr;

void PropPlayer::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _propTexture);
}

PropPlayer::PropPlayer() {
	_renderPriority = 0;

	_health = 2;

	_runSpeed = 0.09f;
	_jumpSpeed = 0.327f;
	_bounceSpeed = 0.4f;
	_gravity = 0.0025f;
	_acceleration = 0.5f;

	animationName = "BigIdle";
}

PropPlayer::~PropPlayer() {}

void PropPlayer::SetRenderPriority(unsigned int rp) {
	_renderPriority = rp;
}

void PropPlayer::SetRunSpeed(float runSpeed) {
	_runSpeed = runSpeed;
}

void PropPlayer::SetJumpSpeed(float jumpSpeed) {
	_jumpSpeed = jumpSpeed;
}

void PropPlayer::SetGravity(float gravity) {
	_gravity = gravity;
}

RECTF PropPlayer::GetBoundingBox(int) const {
	return GameObject::GetBoundingBox(_health >= 2);
}

void PropPlayer::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_propTexture == nullptr) {
		_propTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void PropPlayer::HandleStates() {}

void PropPlayer::HandleCollisionResult(
	LPCOLLISIONEVENT result, 
	D3DXVECTOR2& minTime, 
	D3DXVECTOR2& offset, 
	D3DXVECTOR2& normal, 
	D3DXVECTOR2& relativeDistance) {}

void PropPlayer::MoveLeft() {
	_velocity.x = -_runSpeed;
}

void PropPlayer::MoveRight() {
	_velocity.x = _runSpeed;
}

void PropPlayer::Jump() {
	_velocity.y = -_jumpSpeed;
}

void PropPlayer::SlowFall() {
	_velocity.y *= 0.74f;
}

void PropPlayer::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	Entity::Update(deltaTime, nullptr, collidableTiles);
}

void PropPlayer::Render() {
	_animatedSprite.PlaySpriteAnimation(animationName, _position, _scale);
}

void PropPlayer::Release() {
	_animatedSprite.Release();
	_propTexture = nullptr;
}
#include "../Entity.h"
#include "PropKoopaShell.h"

Texture* PropKoopaShell::_shellTexture = nullptr;

void PropKoopaShell::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _shellTexture);
}

PropKoopaShell::PropKoopaShell() {
	_renderPriority = 1;

	_gravity = 0.0002f;

	animationName = "ShellIdle";
}

PropKoopaShell::~PropKoopaShell() {}

RECTF PropKoopaShell::GetBoundingBox(int) const {
	return GameObject::GetBoundingBox(1);
}

void PropKoopaShell::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_shellTexture == nullptr) {
		_shellTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
}

void PropKoopaShell::HandleStates() {}

void PropKoopaShell::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void PropKoopaShell::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	Entity::Update(deltaTime, nullptr, collidableTiles);
}

void PropKoopaShell::Render() {
	_animatedSprite.PlaySpriteAnimation(animationName, _position);
}

void PropKoopaShell::Release() {
	_animatedSprite.Release();
	_shellTexture = nullptr;
}
#include "../Entity.h"
#include "Portal.h"

Texture* Portal::_portalTexture = nullptr;

void Portal::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _portalTexture);
}

Portal::Portal() {
	isPassThroughable = true;
}

Portal::~Portal() {}

unsigned int Portal::GetSceneID() const {
	return std::stoul(_extraData.front());
}

D3DXVECTOR2 Portal::GetDestination() const {
	return _destination;
}

void Portal::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_portalTexture == nullptr) {
		_portalTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);
	
	if (_extraData.size() > 1) {
		float destX = std::stof(_extraData.at(0));
		float destY = std::stof(_extraData.at(1));

		_destination = { destX, destY };
	}
}

void Portal::HandleStates() {}

void Portal::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void Portal::Update(DWORD, std::vector<Entity*>*, std::vector<Entity*>*, Grid*) {}

void Portal::Render() {
	//_animatedSprite.PlaySpriteAnimation("Portal", _position);
}

void Portal::Release() {
	_animatedSprite.Release();
	_portalTexture = nullptr;
}
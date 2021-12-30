#define _USE_MATH_DEFINES
#include <cmath>

#include "../Entity.h"
#include "Rotodisc.h"

Texture* Rotodisc::_rotoTexture = nullptr;

void Rotodisc::_ParseSprites(std::string line) {
	_animatedSprite.ParseSprites(line, _rotoTexture);
}

Rotodisc::Rotodisc() {
	_renderPriority = 1;

	_direction = 1;
	_radius = 46.0f;
	_angle = 1.0f;
	_roundTripTime = 5;

	isPassThroughable = true;
}

Rotodisc::~Rotodisc() {}

void Rotodisc::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_rotoTexture == nullptr) {
		_rotoTexture = texture;
	}
	Entity::ParseData(dataPath, texture, extraData);

	if (!_extraData.empty()) {
		_centerPoint.x = std::stof(_extraData.at(0));
		_centerPoint.y = std::stof(_extraData.at(1));

		_direction = std::stoi(_extraData.at(2));
	}
}

void Rotodisc::HandleStates() {}

void Rotodisc::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void Rotodisc::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	//https://gamedev.stackexchange.com/questions/67206/how-to-set-speed-of-circular-motion

	const float RIGHT_ANGLE = 90.0f;
	_angle += 2 * static_cast<float>(M_PI) / (deltaTime * _roundTripTime);
	_position.x = _centerPoint.x + (_radius * cos(RIGHT_ANGLE - _direction * _angle));
	_position.y = _centerPoint.y + (_radius * sin(RIGHT_ANGLE - _direction * _angle));
	Entity::Update(deltaTime);
}

void Rotodisc::Render() {
	_animatedSprite.PlaySpriteAnimation("Rotodisc", _position);
}

void Rotodisc::Release() {
	_animatedSprite.Release();
	_rotoTexture = nullptr;
}
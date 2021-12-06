#include "../GlobalUtil.h"
#include "../Game.h"
#include "../audio/AudioService.h"
#include "../Camera.h"
#include "../Entity.h"
#include "MovingCeiling.h"

Texture* MovingCeiling::_ceilingTexture = nullptr;
D3DX10_SPRITE MovingCeiling::_sprite;

void MovingCeiling::_ParseSprites(std::string) {}

void MovingCeiling::_ParseMovingCeiling(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 6) {
		if (tokens.size() == 2) {
			_originalPos.x = std::stof(tokens.at(0));
			_originalPos.y = std::stof(tokens.at(1));
		}

		return;
	}

	RECT spriteBound;
	spriteBound.left = std::stoi(tokens.at(0));
	spriteBound.top = std::stoi(tokens.at(1));
	spriteBound.right = std::stoi(tokens.at(2));
	spriteBound.bottom = std::stoi(tokens.at(3));

	float offsetX = floor((std::stof(tokens.at(4)) - _originalPos.x) / _SPRITE_WIDTH);
	float offsetY = floor((std::stof(tokens.at(5)) - _originalPos.y) / _SPRITE_HEIGHT);

	CeilStruct ceilStruct;
	ceilStruct.spriteBound = spriteBound;
	ceilStruct.offsetX = offsetX;
	ceilStruct.offsetY = offsetY;

	_sprites.emplace_back(std::move(ceilStruct));
}

bool MovingCeiling::_IsInViewport(CeilStruct ceilStruct, RECTF viewport) {
	float spritePosX = _position.x + _SPRITE_WIDTH * ceilStruct.offsetX;
	float spritePosY = _position.y + _SPRITE_HEIGHT * ceilStruct.offsetY;
	
	float spriteWidth = spritePosX + ceilStruct.spriteBound.right;
	float spriteHeight = spritePosY + ceilStruct.spriteBound.bottom;
	if (spriteWidth >= viewport.left &&
		spriteHeight >= viewport.top &&
		spritePosX <= viewport.right &&
		spritePosY <= viewport.bottom)
	{
		return true;
	}

	return false;
}

void MovingCeiling::_ScaleSprite(const RECT& spriteBound) {
	_sprite.TexCoord.x = spriteBound.left / static_cast<float>(_ceilingTexture->width);
	_sprite.TexCoord.y = spriteBound.top / static_cast<float>(_ceilingTexture->height);

	int spriteWidth = spriteBound.right - spriteBound.left;
	int spriteHeight = spriteBound.bottom - spriteBound.top;
	_sprite.TexSize.x = spriteWidth / static_cast<float>(_ceilingTexture->width);
	_sprite.TexSize.y = spriteHeight / static_cast<float>(_ceilingTexture->height);
	_sprite.ColorModulate = { 1.0f, 1.0f, 1.0f, 1.0f };
	_sprite.TextureIndex = 0;

	D3DXMatrixScaling(&_scaleMatrix, static_cast<float>(spriteWidth), static_cast<float>(spriteHeight), 1.0f);
}

MovingCeiling::MovingCeiling() {
	_renderPriority = 1;
	_runSpeed = 0.02f;

	_idleTime = 500;
}

MovingCeiling::~MovingCeiling() {}

bool MovingCeiling::IsIdling() const {
	return _idleStart != 0;
}

void MovingCeiling::StartIdleTimer() {
	_idleStart = static_cast<DWORD>(GetTickCount64());
}

void MovingCeiling::ParseData(
	std::string dataPath, 
	Texture*& texture, 
	std::vector<std::string> extraData) 
{
	if (_ceilingTexture == nullptr) {
		_ceilingTexture = texture;

		_sprite.pTexture = texture->resourceView;
	}
	Entity::ParseData(dataPath, texture, extraData);

	std::ifstream readFile;
	readFile.open(dataPath, std::ios::in);

	if (!readFile.is_open()) {
		char debug[100];
		sprintf_s(debug, "[MOVINGCEILING] Failed to read data for object with ID: %d\n", _objectType);
		OutputDebugStringA(debug);
		return;
	}

	bool isInMovingCeiling = false;
	char str[GlobalUtil::MAX_FILE_LINE];
	while (readFile.getline(str, GlobalUtil::MAX_FILE_LINE)) {
		std::string line(str);

		if (line.empty() || line.front() == '#') {
			continue;
		}

		if (line == "[/]") {
			isInMovingCeiling = false;
			continue;
		}

		if (line == "[MOVINGCEILING]") {
			isInMovingCeiling = true;
			continue;
		}

		if (isInMovingCeiling) {
			_ParseMovingCeiling(line);
		}
	}

	readFile.close();
}

void MovingCeiling::HandleStates() {}

void MovingCeiling::HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) {}

void MovingCeiling::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (isMoving) {
		_position.x = _originalPos.x;
		
		if (!IsIdling()) {
			if (_position.y < _originalPos.y) {
				StartIdleTimer();
				_position.y = _originalPos.y;
				_normal.y = 1.0f;
			}
			else if (_position.y > _originalPos.y + _MAX_HEIGHT) {
				StartIdleTimer();
				_position.y = _originalPos.y + _MAX_HEIGHT;
				_normal.y = -1.0f;

				AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_THWOMP);
			}

			_velocity.y = _runSpeed * _normal.y;
		}
		else {
			_velocity.y = 0.0f;
		}
	}
	
	if (IsIdling() && GetTickCount64() - _idleStart > _idleTime) {
		_idleStart = 0;
	}

	_activeSprites.clear();
	for (const auto& sprite : _sprites) {
		if (_IsInViewport(sprite, Camera::GetInstance()->GetViewport())) {
			_activeSprites.emplace_back(sprite);
		}
	}
	
	Entity::Update(deltaTime);
}

void MovingCeiling::Render() {
	for (const auto& sprite : _activeSprites) {
		float spritePosX = _position.x + _SPRITE_WIDTH * sprite.offsetX;
		float spritePosY = _position.y + _SPRITE_HEIGHT * sprite.offsetY;

		float x = spritePosX - Camera::GetInstance()->GetPosition().x;
		float y = (Game::GetInstance()->GetBackBufferHeight() - spritePosY) + Camera::GetInstance()->GetPosition().y;
		D3DXVECTOR2 spritePosition = { floor(x), floor(y) };

		_ScaleSprite(sprite.spriteBound);

		D3DXMATRIX translationMatrix;
		D3DXMatrixTranslation(&translationMatrix, spritePosition.x, spritePosition.y, 0.1f);
		_sprite.matWorld = _scaleMatrix * translationMatrix;

		GlobalUtil::spriteHandler->DrawSpritesImmediate(&_sprite, 1, 0, 0);
	}
}

void MovingCeiling::Release() {
	_animatedSprite.Release();
	_activeSprites.clear();
	_sprites.clear();
	_sprite.pTexture = nullptr;
	_ceilingTexture = nullptr;
}
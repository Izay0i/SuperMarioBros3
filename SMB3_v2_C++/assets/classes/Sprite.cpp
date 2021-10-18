#include "GlobalUtil.h"
#include "Game.h"
#include "Sprite.h"

void Sprite::_ScaleSprite(const RECT& spriteBound, D3DXVECTOR2 scale, unsigned int alpha) {
	_sprite.TexCoord.x = spriteBound.left / static_cast<float>(_texture->width);
	_sprite.TexCoord.y = spriteBound.top / static_cast<float>(_texture->height);

	int spriteWidth = spriteBound.right - spriteBound.left;
	int spriteHeight = spriteBound.bottom - spriteBound.top;
	_sprite.TexSize.x = spriteWidth / static_cast<float>(_texture->width);
	_sprite.TexSize.y = spriteHeight / static_cast<float>(_texture->height);
	_sprite.ColorModulate = { 1.0f, 1.0f, 1.0f, alpha / 255.0f };
	_sprite.TextureIndex = 0;

	D3DXMatrixScaling(&_scaleMatrix, static_cast<float>(spriteWidth) * scale.x, static_cast<float>(spriteHeight) * scale.y, 1.0f);
}

Sprite::Sprite(Texture*& texture, RECT spriteBound, unsigned int totalFrames, int animationSpeed) {
	_currentFrame = -1;
	_texture = texture;
	_sprite.pTexture = texture->resourceView;
	_bounds.emplace_back(spriteBound);
	_totalFrames = totalFrames;
	_animationSpeed = animationSpeed;
	_cameraInstance = Camera::GetInstance();
}

Sprite::~Sprite() {}

void Sprite::AddSpriteBound(RECT spriteBound) {
	_bounds.emplace_back(spriteBound);
}

void Sprite::DrawSprite(D3DXVECTOR2 position, D3DXVECTOR2 scale, unsigned int alpha) {
	DWORD now = static_cast<DWORD>(GetTickCount64());
	//Init
	if (_currentFrame == -1) {
		_currentFrame = 0;
		_lastFrameTime = now;
	}
	//Loop
	else {
		DWORD animationSpeed = static_cast<DWORD>(_animationSpeed);

		if (now - _lastFrameTime > animationSpeed) {
			++_currentFrame;
			_lastFrameTime = now;

			if (_currentFrame == _totalFrames) {
				_currentFrame = 0;
			}
		}
	}
	
	float x = position.x - _cameraInstance->GetPosition().x;
	float y = (Game::GetInstance()->GetBackBufferHeight() - position.y) + _cameraInstance->GetPosition().y;
	D3DXVECTOR2 spritePosition = { floor(x), floor(y) };

	_ScaleSprite(_bounds.at(_currentFrame), scale, alpha);

	D3DXMATRIX translationMatrix;
	D3DXMatrixTranslation(&translationMatrix, spritePosition.x, spritePosition.y, 0.1f);
	_sprite.matWorld = _scaleMatrix * translationMatrix;

	GlobalUtil::spriteHandler->DrawSpritesImmediate(&_sprite, 1, 0, 0);
}

void Sprite::Release() {
	_bounds.clear();
	_sprite.pTexture = nullptr;
	_texture = nullptr;
}
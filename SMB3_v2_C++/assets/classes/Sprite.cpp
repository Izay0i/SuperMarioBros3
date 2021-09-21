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

//Direct3D 10
Sprite::Sprite(Texture*& texture, RECT spriteBound, unsigned int totalFrames, int animationSpeed) {
	_currentFrame = -1;
	_texture = texture;
	_sprite.pTexture = texture->resourceView;
	_bounds.emplace_back(spriteBound);
	_totalFrames = totalFrames;
	_animationSpeed = animationSpeed;
	_cameraInstance = Camera::GetInstance();
}
//CHANGED
//Sprite::Sprite(const LPDIRECT3DTEXTURE9& spriteTexture, RECT spriteBound, unsigned int totalFrames, int animationSpeed) {
//	_currentFrame = -1;
//	_spriteTexture = spriteTexture;
//	_bounds.emplace_back(spriteBound);
//	_totalFrames = totalFrames;
//	_animationSpeed = animationSpeed;
//	_cameraInstance = Camera::GetInstance();
//}
//END

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

	//Direct3D 10
	float x = position.x - _cameraInstance->GetPosition().x;
	float y = (Game::GetInstance()->GetBackBufferHeight() - position.y) + _cameraInstance->GetPosition().y;
	D3DXVECTOR2 spritePosition = { floor(x), floor(y) };

	_ScaleSprite(_bounds.at(_currentFrame), scale, alpha);

	D3DXMATRIX translationMatrix;
	D3DXMatrixTranslation(&translationMatrix, spritePosition.x, spritePosition.y, 0.1f);
	_sprite.matWorld = _scaleMatrix * translationMatrix;

	GlobalUtil::spriteHandler->DrawSpritesImmediate(&_sprite, 1, 0, 0);

	//CHANGED
	//float x = position.x - _cameraInstance->GetPosition().x;
	//float y = position.y - _cameraInstance->GetPosition().y;
	////Removes the blur
	//D3DXVECTOR2 spritePosition = { floor(x), floor(y) };

	////Voodoo magic
	//D3DXMATRIX matrix;
	//D3DXVECTOR2 drawCenter = D3DXVECTOR2(8.0f, 8.0f);
	//D3DXMatrixTransformation2D(&matrix, &drawCenter, 0.0f, &scale, nullptr, 0.0f, &spritePosition);
	//GlobalUtil::spriteHandler->SetTransform(&matrix);
	//GlobalUtil::spriteHandler->Draw(
	//	_spriteTexture,
	//	&_bounds.at(_currentFrame),
	//	nullptr,
	//	nullptr,
	//	D3DCOLOR_ARGB(alpha, 255, 255, 255)
	//);
	//END
}

void Sprite::Release() {
	_bounds.clear();
	//Direct3D 10
	_sprite.pTexture = nullptr;
	_texture = nullptr;
	//REMOVED
	//_spriteTexture = nullptr;
	//END
}
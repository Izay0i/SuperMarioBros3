#include "Sprite.h"

Sprite::Sprite(const LPDIRECT3DTEXTURE9& spriteTexture, unsigned int totalFrames) {
	_currentFrame = -1;
	_spriteTexture = spriteTexture;
	_totalFrames = totalFrames;
	_animationSpeed = 0;
	_cameraInstance = Camera::GetInstance();
}

Sprite::Sprite(const LPDIRECT3DTEXTURE9& spriteTexture, RECT spriteBound, unsigned int totalFrames, int animationSpeed) {
	_currentFrame = -1;
	_spriteTexture = spriteTexture;
	_bounds.emplace_back(spriteBound);
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
	float y = position.y - _cameraInstance->GetPosition().y;
	//Removes the blur
	D3DXVECTOR2 spritePosition = D3DXVECTOR2(floor(x), floor(y));

	D3DXMATRIX matrix;
	D3DXVECTOR2 drawCenter = D3DXVECTOR2(8.0f, 8.0f);

	//Voodoo magic
	D3DXMatrixTransformation2D(&matrix, &drawCenter, 0.0f, &scale, nullptr, 0.0f, &spritePosition);
	GlobalUtil::spriteHandler->SetTransform(&matrix);
	GlobalUtil::spriteHandler->Draw(
		_spriteTexture,
		&_bounds.at(_currentFrame),
		nullptr,
		nullptr,
		D3DCOLOR_ARGB(alpha, 255, 255, 255)
	);
}

void Sprite::Release() {
	_bounds.clear();

	if (_spriteTexture != nullptr) {
		_spriteTexture->Release();
	}
}
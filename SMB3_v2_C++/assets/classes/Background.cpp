#include "GlobalUtil.h"
#include "Camera.h"
#include "Background.h"

LPDIRECT3DTEXTURE9 Background::_backgroundTexture = nullptr;

Background::Background(const LPDIRECT3DTEXTURE9& backgroundTexture) {
	if (_backgroundTexture == nullptr) {
		_backgroundTexture = backgroundTexture;
	}
}

Background::~Background() {}

void Background::AddSprite(RECT spriteBound, D3DXVECTOR2 position) {
	_sprites.emplace_back(std::make_pair(spriteBound, position));
}

void Background::Render() {
	//The old method was slow so I removed it
	for (const auto& sprite : _sprites) {
		float x = sprite.second.x - Camera::GetInstance()->GetPosition().x;
		float y = sprite.second.y - Camera::GetInstance()->GetPosition().y;
		D3DXVECTOR2 spritePosition = D3DXVECTOR2(floor(x), floor(y));

		D3DXMATRIX matrix;
		D3DXVECTOR2 scale(1.0f, 1.0f);
		D3DXMatrixTransformation2D(&matrix, nullptr, 0.0f, &scale, nullptr, 0.0f, &spritePosition);
		GlobalUtil::spriteHandler->SetTransform(&matrix);
		GlobalUtil::spriteHandler->Draw(
			_backgroundTexture,
			&sprite.first,
			nullptr,
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255)
		);
	}
}

void Background::Release() {
	_sprites.clear();
	_backgroundTexture = nullptr;
}
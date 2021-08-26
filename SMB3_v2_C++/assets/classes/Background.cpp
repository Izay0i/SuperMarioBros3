#include "GlobalUtil.h"
#include "Background.h"

Background::Background(const LPDIRECT3DTEXTURE9& backgroundTexture, unsigned int totalFrames) {
	_backgroundSprite = new Sprite(backgroundTexture, { -1, -1, -1, -1 }, totalFrames, 0);
}

Background::~Background() {}

void Background::AddSprite(RECT spriteBound, D3DXVECTOR2 position) {
	_positions.emplace_back(position);
	_backgroundSprite->AddSpriteBound(spriteBound);
}

void Background::Render() {
	for (const auto& position : _positions) {
		_backgroundSprite->DrawSprite(position, D3DXVECTOR2(1.0f, 1.0f), 255);
	}
}

void Background::Release() {
	_positions.clear();
	_backgroundSprite = nullptr;
}
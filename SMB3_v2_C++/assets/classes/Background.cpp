#include "GlobalUtil.h"
#include "Game.h"
#include "Camera.h"
#include "Background.h"

Texture* Background::_backgroundTexture = nullptr;

bool Background::_IsInViewport(std::pair<RECT, D3DXVECTOR2> sprite, RECTF viewport) {
	float spriteWidth = sprite.second.x + sprite.first.right;
	float spriteHeight = sprite.second.y + sprite.first.bottom;
	if (spriteWidth >= viewport.left && 
		spriteHeight >= viewport.top && 
		sprite.second.x <= viewport.right && 
		sprite.second.y <= viewport.bottom) 
	{
		return true;
	}

	return false;
}

void Background::_ScaleSprite(const RECT& spriteBound) {
	_sprite.TexCoord.x = spriteBound.left / static_cast<float>(_backgroundTexture->width);
	_sprite.TexCoord.y = spriteBound.top / static_cast<float>(_backgroundTexture->height);

	int spriteWidth = spriteBound.right - spriteBound.left;
	int spriteHeight = spriteBound.bottom - spriteBound.top;
	_sprite.TexSize.x = spriteWidth / static_cast<float>(_backgroundTexture->width);
	_sprite.TexSize.y = spriteHeight / static_cast<float>(_backgroundTexture->height);
	_sprite.ColorModulate = { 1.0f, 1.0f, 1.0f, 1.0f };
	_sprite.TextureIndex = 0;

	D3DXMatrixScaling(&_scaleMatrix, static_cast<float>(spriteWidth), static_cast<float>(spriteHeight), 1.0f);
}

Background::Background(Texture*& backgroundTexture) {
	if (_backgroundTexture == nullptr) {
		_backgroundTexture = backgroundTexture;

		_sprite.pTexture = backgroundTexture->resourceView;
	}
}

Background::~Background() {}

void Background::AddSprite(RECT spriteBound, D3DXVECTOR2 position) {
	_sprites.emplace_back(std::make_pair(spriteBound, position));
}

void Background::Update() {
	_activeSprites.clear();

	for (const auto& sprite : _sprites) {
		if (_IsInViewport(sprite, Camera::GetInstance()->GetViewport())) {
			_activeSprites.emplace_back(sprite);
		}
	}
}

void Background::Render() {
	for (const auto& sprite : _activeSprites) {
		float x = sprite.second.x - Camera::GetInstance()->GetPosition().x;
		float y = (Game::GetInstance()->GetBackBufferHeight() - sprite.second.y) + Camera::GetInstance()->GetPosition().y;
		D3DXVECTOR2 spritePosition = { floor(x), floor(y) };

		_ScaleSprite(sprite.first);

		D3DXMATRIX translationMatrix;
		D3DXMatrixTranslation(&translationMatrix, spritePosition.x, spritePosition.y, 0.1f);
		_sprite.matWorld = _scaleMatrix * translationMatrix;

		GlobalUtil::spriteHandler->DrawSpritesImmediate(&_sprite, 1, 0, 0);
	}
}

void Background::BruteForceRender() {
	for (const auto& sprite : _sprites) {
		float x = sprite.second.x - Camera::GetInstance()->GetPosition().x;
		float y = (Game::GetInstance()->GetBackBufferHeight() - sprite.second.y) + Camera::GetInstance()->GetPosition().y;
		D3DXVECTOR2 spritePosition = { floor(x), floor(y) };

		_ScaleSprite(sprite.first);

		D3DXMATRIX translationMatrix;
		D3DXMatrixTranslation(&translationMatrix, spritePosition.x, spritePosition.y, 0.1f);
		_sprite.matWorld = _scaleMatrix * translationMatrix;

		GlobalUtil::spriteHandler->DrawSpritesImmediate(&_sprite, 1, 0, 0);
	}
}

void Background::Release() {
	_activeSprites.clear();
	_sprites.clear();
	_sprite.pTexture = nullptr;
	_backgroundTexture = nullptr;
}
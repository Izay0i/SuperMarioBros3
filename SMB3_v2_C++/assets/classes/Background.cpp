#include "GlobalUtil.h"
#include "Game.h"
#include "Camera.h"
#include "Background.h"

Texture* Background::_backgroundTexture = nullptr;

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

void Background::Render() {
	//Direct3D 10
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

	//CHANGED
	////The old method was slow so I removed it
	//for (const auto& sprite : _sprites) {
	//	float x = sprite.second.x - Camera::GetInstance()->GetPosition().x;
	//	float y = sprite.second.y - Camera::GetInstance()->GetPosition().y;
	//	D3DXVECTOR2 spritePosition = D3DXVECTOR2(floor(x), floor(y));

	//	D3DXMATRIX matrix;
	//	D3DXVECTOR2 scale(1.0f, 1.0f);
	//	D3DXMatrixTransformation2D(&matrix, nullptr, 0.0f, &scale, nullptr, 0.0f, &spritePosition);
	//	GlobalUtil::spriteHandler->SetTransform(&matrix);
	//	GlobalUtil::spriteHandler->Draw(
	//		_backgroundTexture,
	//		&sprite.first,
	//		nullptr,
	//		nullptr,
	//		D3DCOLOR_ARGB(255, 255, 255, 255)
	//	);
	//}
	//END
}

void Background::Release() {
	_sprites.clear();
	_sprite.pTexture = nullptr;
	_backgroundTexture = nullptr;
}
#include "AnimatedSprite.h"

Sprite* AnimatedSprite::_GetAnimation(std::string animationName) {
	return _sprites[animationName];
}

bool AnimatedSprite::_HasAnimation(std::string animationName) const {
	return _sprites.find(animationName) != _sprites.end();
}

void AnimatedSprite::_AddSpriteBound(std::string animationName, RECT spriteBound) {
	_sprites[animationName]->AddSpriteBound(spriteBound);
}

void AnimatedSprite::ParseSprites(std::string line, LPDIRECT3DTEXTURE9& spriteTexture, D3DCOLOR colorKey) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 5) {
		return;
	}

	RECT spriteBound;
	spriteBound.left = std::stoi(tokens.at(1));
	spriteBound.top = std::stoi(tokens.at(2));
	spriteBound.right = std::stoi(tokens.at(3));
	spriteBound.bottom = std::stoi(tokens.at(4));

	if (!_HasAnimation(tokens.at(0))) {
		int totalFrames = std::stoi(tokens.at(5));
		int animationSpeed = std::stoi(tokens.at(6));
		_sprites.insert(
			std::make_pair(
				tokens.at(0), 
				new Sprite(spriteTexture, spriteBound, totalFrames, animationSpeed, colorKey)
			)
		);
	}
	else {
		_sprites[tokens.at(0)]->AddSpriteBound(spriteBound);
	}
}

void AnimatedSprite::PlaySpriteAnimation(std::string animationName, D3DXVECTOR2 position, D3DXVECTOR2 scale, unsigned int alpha) {
	if (!_HasAnimation(animationName)) {
		char debugStr[100];
		sprintf_s(debugStr, "[ANIMATED SPRITE] Animation %s does not exist\n", animationName.c_str());
		OutputDebugStringW(GlobalUtil::ToLPCWSTR(debugStr));
		return;
	}

	_sprites[animationName]->DrawSprite(position, scale, alpha);
}

void AnimatedSprite::Release() {
	for (auto sprite : _sprites) {
		sprite.second->Release();
		delete sprite.second;
	}
	_sprites.clear();
}
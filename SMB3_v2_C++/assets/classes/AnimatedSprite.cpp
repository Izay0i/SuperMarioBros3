#include "AnimatedSprite.h"

bool AnimatedSprite::_HasAnimation(std::string animationName) const {
	return _sprites.find(animationName) != _sprites.end();
}

void AnimatedSprite::ParseSprites(std::string line, Texture*& texture) {
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
		if (tokens.size() < 7) {
			char debug[100];
			sprintf_s(debug, "[ANIMATED SPRITE] Insufficient parameters from animation named %s\n", tokens.at(0).c_str());
			OutputDebugStringA(debug);
			return;
		}

		unsigned int totalFrames = std::stoul(tokens.at(5));
		int animationSpeed = std::stoul(tokens.at(6));

		Sprite* sprite = new Sprite(texture, spriteBound, totalFrames, animationSpeed);
		_sprites.insert(std::make_pair(tokens.at(0), sprite));
	}
	else {
		_sprites[tokens.at(0)]->AddSpriteBound(spriteBound);
	}
}

void AnimatedSprite::PlaySpriteAnimation(std::string animationName, D3DXVECTOR2 position, D3DXVECTOR2 scale, unsigned int alpha) {
	if (!_HasAnimation(animationName)) {
		char debug[100];
		sprintf_s(debug, "[ANIMATED SPRITE] Animation named %s does not exist\n", animationName.c_str());
		OutputDebugStringA(debug);
		return;
	}

	_sprites[animationName]->DrawSprite(position, scale, alpha);
}

void AnimatedSprite::Release() {
	for (auto& sprite : _sprites) {
		sprite.second->Release();
		delete sprite.second;
	}
	_sprites.clear();
}
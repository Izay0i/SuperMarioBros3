#pragma once

#include "Sprite.h"

#include <string>
#include <unordered_map>

class AnimatedSprite {
private:
	std::unordered_map<std::string, Sprite*> _sprites;

	bool _HasAnimation(std::string) const;

public:
	void ParseSprites(std::string, Texture*&);
	
	//Animation name, position, scale, alpha
	void PlaySpriteAnimation(std::string, D3DXVECTOR2, D3DXVECTOR2 = D3DXVECTOR2(1.0f, 1.0f), unsigned int = 255);

	void Release();
};
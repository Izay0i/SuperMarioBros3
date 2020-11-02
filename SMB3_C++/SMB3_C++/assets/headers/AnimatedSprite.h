#pragma once

#include <string>
#include <unordered_map>

#include <d3dx9.h>

#include "SceneManager.h"
#include "Sprite.h"

class Sprite;

class AnimatedSprite {
private:
	std::unordered_map<std::string, Sprite*> sprites;

	Sprite* GetAnimation(std::string);

	bool HasAnimation(std::string);

	void AddBound(RECT, std::string);

public:	
	void ParseSprites(std::string);
	
	void PlayAnimation(std::string);
};
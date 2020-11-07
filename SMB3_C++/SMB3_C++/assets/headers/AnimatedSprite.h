#pragma once

#include <string>
#include <unordered_map>

#include <d3dx9.h>

#include "Sprite.h"

class Sprite;

class AnimatedSprite {
private:
	std::unordered_map<std::string, Sprite*> sprites;

	Sprite* GetAnimation(std::string);

	LPDIRECT3DDEVICE9 directDevice;
	LPD3DXSPRITE spriteHandler;

	bool HasAnimation(std::string);

	void AddBound(RECT, std::string);

public:

	void SetDevice(LPDIRECT3DDEVICE9& dev) { directDevice = dev; }
	LPDIRECT3DDEVICE9 GetDevice() { return directDevice; }

	void SetSpriteHandler(LPD3DXSPRITE& handler) { spriteHandler = handler; }
	LPD3DXSPRITE GetSpriteHandler() { return spriteHandler; }

	void ParseSprites(std::string, std::string, D3DCOLOR);
	
	void PlayAnimation(std::string, D3DXVECTOR3, D3DXVECTOR2 = D3DXVECTOR2(1.0f, 1.0f));
};
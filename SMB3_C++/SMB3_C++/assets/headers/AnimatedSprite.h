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

	static LPDIRECT3DDEVICE9 directDevice;
	static LPD3DXSPRITE spriteHandler;

	bool HasAnimation(std::string) const;

	void AddBound(RECT, std::string);

public:
	static void SetDevice(LPDIRECT3DDEVICE9&);
	static LPDIRECT3DDEVICE9 GetDevice();

	static void SetSpriteHandler(LPD3DXSPRITE&);
	static LPD3DXSPRITE GetSpriteHandler();

	void ParseSprites(std::string, LPDIRECT3DTEXTURE9, D3DCOLOR);
	
	//animation name, position, scale, alpha
	void PlayAnimation(std::string, D3DXVECTOR3, D3DXVECTOR2 = D3DXVECTOR2(1.0f, 1.0f), unsigned int = 255);
};
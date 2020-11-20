#pragma once

#include <string>
#include <vector>

#include <d3dx9.h>

#include "Camera.h"
#include "Util.h"

class Sprite {
private:
	int totalFrames, currentFrame = -1;
	int animationSpeed;

	std::vector<RECT> bounds;

	DWORD lastFrameTime;

	LPDIRECT3DTEXTURE9 texture;
	D3DCOLOR colorKey;

	static LPDIRECT3DDEVICE9 directDevice;
	static LPD3DXSPRITE spriteHandler;

public:
	Sprite(LPDIRECT3DTEXTURE9, RECT, int, int, D3DCOLOR);

	static void SetDevice(LPDIRECT3DDEVICE9& dev) { if (!directDevice) { directDevice = dev; } }
	LPDIRECT3DDEVICE9 GetDevice() { return directDevice; }

	static void SetSpriteHandler(LPD3DXSPRITE& handler) { if (!spriteHandler) { spriteHandler = handler; } }
	LPD3DXSPRITE GetSpriteHandler() { return spriteHandler; }

	void AddBound(RECT);

	//position, scale, center pos to scale from
	void Draw(D3DXVECTOR3, D3DXVECTOR2 = D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR2 = D3DXVECTOR2(8, 8));

	void Release();
};
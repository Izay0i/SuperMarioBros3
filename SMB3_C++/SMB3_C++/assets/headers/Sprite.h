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
	static LPCWSTR filePath;

	static LPDIRECT3DTEXTURE9 texture;
	
	static D3DCOLOR colorKey;

	static LPDIRECT3DDEVICE9 directDevice;
	static LPD3DXSPRITE spriteHandler;

	void LoadTexture();

public:
	Sprite(std::string, RECT, int, float, D3DCOLOR, LPDIRECT3DDEVICE9&, LPD3DXSPRITE&);

	void AddBound(RECT);

	void Draw(D3DXVECTOR3, D3DXVECTOR2 = D3DXVECTOR2(1.0f, 1.0f));

	void Release();
};
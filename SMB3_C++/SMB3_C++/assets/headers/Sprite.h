#pragma once

#include <string>
#include <vector>

#include <d3dx9.h>

#include "Util.h"
#include "Game.h"

class Sprite {
private:
	int totalFrames, currentFrame = -1;
	int animationSpeed;

	std::vector<RECT> bounds;

	DWORD lastFrameTime;
	static LPCWSTR filePath;

	static LPDIRECT3DTEXTURE9 texture;
	
	static D3DCOLOR colorKey;

	void LoadTexture();

public:
	Sprite(std::string, RECT, int, float, D3DCOLOR);

	void AddBound(RECT);

	void Draw(D3DXVECTOR3);

	void Release();
};
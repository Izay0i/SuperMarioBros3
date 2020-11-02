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
	LPCWSTR filePath;

	LPDIRECT3DTEXTURE9 texture;
	
	D3DCOLOR colorKey;

	D3DXVECTOR3 position;
	D3DXVECTOR2 rotation;
	D3DXVECTOR2 translation;
	D3DXVECTOR2 scale;

	D3DXMATRIX matrix;

	void LoadTexture();

public:
	Sprite(std::string, RECT, int, float, D3DCOLOR);

	void AddBound(RECT);

	void Draw();

	void Release();
};
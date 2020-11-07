#pragma once

#include <vector>
#include <string>

#include <d3dx9.h>

#include <Windows.h>

#include "Camera.h"
#include "Util.h"

class Background {
private:
	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	LPDIRECT3DDEVICE9 directDevice;
	LPD3DXSPRITE spriteHandler;

	std::vector<std::pair<RECT, D3DXVECTOR3>> images;

public:
	void SetDevice(LPDIRECT3DDEVICE9& dev) { directDevice = dev; }
	LPDIRECT3DDEVICE9 GetDevice() { return directDevice; }

	void SetSpriteHandler(LPD3DXSPRITE& handler) { spriteHandler = handler; }
	LPD3DXSPRITE GetSpriteHandler() { return spriteHandler; }

	void LoadTexture(std::string, D3DCOLOR);

	void AddImage(RECT, D3DXVECTOR3);

	void DrawBackground();

	void Release();
};
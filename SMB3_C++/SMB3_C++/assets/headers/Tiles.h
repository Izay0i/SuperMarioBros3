#pragma once

#include <vector>
#include <string>

#include <d3dx9.h>

#include <Windows.h>

#include "Game.h"
#include "Util.h"

class Tiles {
private:
	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	std::vector<std::pair<RECT, D3DXVECTOR3>> images;

	HitBox hitbox;

public:
	void LoadTexture(std::string, D3DCOLOR);

	void AddHitBox(RECTF);
	void AddImage(RECT, D3DXVECTOR3);

	void Render();

	void Release();
};
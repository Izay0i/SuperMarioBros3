#pragma once

#include <vector>
#include <string>

#include <d3dx9.h>

#include <Windows.h>

#include "GameObject.h"
#include "Camera.h"
#include "Util.h"

class Background : public GameObject {
private:
	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	std::vector<std::pair<RECT, D3DXVECTOR3>> images;

public:
	void LoadTexture(std::string, D3DCOLOR);

	void AddImage(RECT, D3DXVECTOR3);

	void Update(DWORD) override {}
	void Render() override;

	void Release();
};
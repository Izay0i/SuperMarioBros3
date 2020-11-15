#pragma once

#include <vector>
#include <string>

#include <d3dx9.h>

#include <Windows.h>

#include "GameObject.h"
#include "HitBox.h"
#include "Camera.h"
#include "Util.h"

class HitBox;

class Tiles : public GameObject {
private:
	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	int spritesArrID;
	std::vector<std::pair<RECT, D3DXVECTOR3>> images;

public:
	Tiles();
	~Tiles();

	void LoadTexture(std::string, D3DCOLOR);

	void AddHitBox(RECTF);
	void AddImage(RECT, D3DXVECTOR3);
	
	void SetSpritesArrID(int id) { spritesArrID = id; }
	int GetSpritesArrID() { return spritesArrID; }

	RECTF GetBoundingBox(int id = 0) const override {
		RECTF bound;
		bound.left = position.x + 1;
		bound.top = position.y + 1;
		bound.right = position.x + hitBox.GetWidth(id) - 1;
		bound.bottom = position.y + hitBox.GetHeight(id) - 1;

		return bound;
	}

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override {}
	void Render() override;

	void Release() override;
};
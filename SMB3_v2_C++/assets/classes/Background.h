#pragma once

#include "GlobalUtil.h"
#include "GameObject.h"
#include "Sprite.h"

class Background : public GameObject {
private:
	static Background* _backgroundInstance;

	static LPDIRECT3DTEXTURE9 _backgroundTexture;

	std::vector<std::pair<RECT, D3DXVECTOR2>> _sprites;

	Background();
	~Background();

public:
	static Background* GetInstance();

	void LoadTexture(std::string, D3DCOLOR);
	void AddSprite(RECT, D3DXVECTOR2);

	void Render() override;

	void Release() override;
};
#pragma once

#include "GameObject.h"

class Background : public GameObject {
private:
	static LPDIRECT3DTEXTURE9 _backgroundTexture;

	std::vector<std::pair<RECT, D3DXVECTOR2>> _sprites;

public:
	Background(const LPDIRECT3DTEXTURE9&);
	~Background();

	void AddSprite(RECT, D3DXVECTOR2);

	void Render() override;

	void Release() override;
};
#pragma once

#include "GameObject.h"

class Background : public GameObject {
private:
	static Texture* _backgroundTexture;

	std::vector<std::pair<RECT, D3DXVECTOR2>> _sprites;

	D3DXMATRIX _scaleMatrix;
	D3DX10_SPRITE _sprite;

	void _ScaleSprite(const RECT&);

public:
	Background(Texture*&);
	~Background();

	void AddSprite(RECT, D3DXVECTOR2);

	void Render() override;

	void Release() override;
};
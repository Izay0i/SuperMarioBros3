#pragma once

#include "GameObject.h"

class Background : public GameObject {
private:
	static Texture* _backgroundTexture;

	std::vector<std::pair<RECT, D3DXVECTOR2>> _activeSprites;
	std::vector<std::pair<RECT, D3DXVECTOR2>> _sprites;

	D3DXMATRIX _scaleMatrix;
	D3DX10_SPRITE _sprite;

	bool _IsInViewport(std::pair<RECT, D3DXVECTOR2>, RECTF);

	void _ScaleSprite(const RECT&);

public:
	Background(Texture*&);
	~Background();

	void AddSprite(RECT, D3DXVECTOR2);

	void Update();
	void Render() override;
	//Renders everything to the screen, disregarding the camera's viewport
	void BruteForceRender();

	void Release() override;
};
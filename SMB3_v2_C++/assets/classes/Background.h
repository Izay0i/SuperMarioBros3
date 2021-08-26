#pragma once

#include "GameObject.h"
#include "Sprite.h"

class Background : public GameObject {
private:
	std::vector<D3DXVECTOR2> _positions;
	
	Sprite* _backgroundSprite;

public:
	//Texture, total frames
	Background(const LPDIRECT3DTEXTURE9&, unsigned int);
	~Background();

	void AddSprite(RECT, D3DXVECTOR2);

	void Render() override;

	void Release() override;
};
#pragma once

#include <d3dx9.h>

class GameObject {
protected:
	float posX, posY;

	D3DXVECTOR3 position;
	D3DXVECTOR2 rotation;
	D3DXVECTOR2 translation;
	D3DXVECTOR2 scale;

	D3DXMATRIX matrix;
	
public:
	virtual ~GameObject() {}

	virtual void Release() = 0;
};
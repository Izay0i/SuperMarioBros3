#pragma once

/*
D3DXMATRIX matrix;
D3DXMatrixIndentity(&matrix);

matrix._22 = -1.0f;
matrix._41 = -cameraPosX;
matrix._42 = 600;

D3DXVECTOR4 viewPortPos;
D3DXVec3Transform(&viewPortPos, &position, &matrix);

D3DXVECTOR3 pos(viewPortPos.x, viewPortPos.y, 0);
D3DXVECTOR3 center(
	static_cast<float>(texture->GetWidth()) / 2,
	static_cast<float>(texture->GetHeight()) / 2,
	0
);

spriteHandler->Draw(
	texture->GetTexture(),
	&sourceRect,
	&center,
	&pos,
	D3DCOLOR_XRGB(255, 255, 255)
);
*/


#include "GameObject.h"

class Camera : GameObject {
private:
	static GameObject* cameraInstance;

public:
	static Camera* GetInstance();

	void Release() override;
};
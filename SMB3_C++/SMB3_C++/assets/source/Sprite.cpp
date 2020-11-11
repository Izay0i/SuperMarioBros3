#include "../headers/Sprite.h"

LPDIRECT3DDEVICE9 Sprite::directDevice = nullptr;
LPD3DXSPRITE Sprite::spriteHandler = nullptr;

Sprite::Sprite(LPDIRECT3DTEXTURE9 texture, RECT bound, int frames, float animSpeed, D3DCOLOR color) {
	this->texture = texture;
	bounds.push_back(bound);
	totalFrames = frames;
	animationSpeed = animSpeed;
	colorKey = color;
}

void Sprite::AddBound(RECT bound) {
	bounds.push_back(bound);
}

void Sprite::Draw(D3DXVECTOR3 position, D3DXVECTOR2 scale) {
	DWORD now = static_cast<DWORD>(GetTickCount64());

	if (currentFrame == -1) {
		currentFrame = 0;
		lastFrameTime = now;
	}
	else {
		DWORD animSpeed = static_cast<DWORD>(animationSpeed);

		if (now - lastFrameTime > animSpeed) {
			++currentFrame;
			lastFrameTime = now;

			if (currentFrame == totalFrames) {
				currentFrame = 0;
			}
		}
	}
	
	int x = static_cast<int>(position.x - Camera::GetInstance()->GetPosition().x);
	int y = static_cast<int>(position.y - Camera::GetInstance()->GetPosition().y);
	D3DXVECTOR2 spritePosition = D3DXVECTOR2(x, y);

	D3DXMATRIX mat;
	D3DXVECTOR2 center(8, 8);

	D3DXMatrixTransformation2D(&mat, &center, 0.0f, &scale, nullptr, 0.0f, &spritePosition);
	spriteHandler->SetTransform(&mat);

	spriteHandler->Draw(
		texture,
		&bounds.at(currentFrame),
		nullptr,
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255)
	);
}

void Sprite::Release() {
	if (texture) {
		texture->Release();
	}

	bounds.clear();
}
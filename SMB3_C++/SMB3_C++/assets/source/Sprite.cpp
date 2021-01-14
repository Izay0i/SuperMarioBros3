#include "../headers/Sprite.h"

LPDIRECT3DDEVICE9 Sprite::directDevice = nullptr;
LPD3DXSPRITE Sprite::spriteHandler = nullptr;

Sprite::Sprite(LPDIRECT3DTEXTURE9 texture, RECT bound, int frames, int animSpeed, D3DCOLOR color) {
	this->texture = texture;
	bounds.push_back(bound);
	totalFrames = frames;
	animationSpeed = animSpeed;
	colorKey = color;
}

void Sprite::AddBound(RECT bound) {
	bounds.push_back(bound);
}

void Sprite::Draw(D3DXVECTOR3 position, D3DXVECTOR2 scale, unsigned int alpha) {
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
	
	float x = position.x - Camera::GetInstance()->GetPosition().x;
	float y = position.y - Camera::GetInstance()->GetPosition().y;
	D3DXVECTOR2 spritePosition = D3DXVECTOR2(floor(x), floor(y));

	D3DXMATRIX mat;
	D3DXVECTOR2 center = D3DXVECTOR2(8, 8);

	D3DXMatrixTransformation2D(&mat, &center, 0.0f, &scale, nullptr, 0.0f, &spritePosition);
	spriteHandler->SetTransform(&mat);

	spriteHandler->Draw(
		texture,
		&bounds.at(currentFrame),
		nullptr,
		nullptr,
		D3DCOLOR_ARGB(alpha, 255, 255, 255)
	);
}

void Sprite::Release() {
	if (texture) {
		texture->Release();
	}

	bounds.clear();
}
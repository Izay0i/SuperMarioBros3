#include "../headers/Sprite.h"

LPCWSTR Sprite::filePath = nullptr;
LPDIRECT3DTEXTURE9 Sprite::texture = nullptr;
D3DCOLOR Sprite::colorKey = D3DCOLOR_XRGB(0, 0, 0);

void Sprite::LoadTexture() {
	if (!texture) {
		HRESULT hResult;
		D3DXIMAGE_INFO imageInfo;

		hResult = D3DXGetImageInfoFromFile(filePath, &imageInfo);
		if (hResult != D3D_OK) {
			OutputDebugStringA("Failed to get image info\n");
			return;
		}

		hResult = D3DXCreateTextureFromFileEx(
			Game::GetInstance()->GetDevice(),
			filePath,
			imageInfo.Width,
			imageInfo.Height,
			1,
			D3DUSAGE_DYNAMIC,
			D3DFMT_UNKNOWN,
			D3DPOOL_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			colorKey,
			&imageInfo,
			nullptr,
			&texture
			);
		if (hResult != D3D_OK) {
			OutputDebugStringA("Failed to create texture from file\n");
			return;
		}
	}
}

Sprite::Sprite(std::string path, RECT bound, int frames, float animSpeed, D3DCOLOR color) {
	filePath = Util::ToLPCWSTR(path);
	bounds.push_back(bound);
	totalFrames = frames;
	animationSpeed = animSpeed;
	colorKey = color;

	LoadTexture();
}

void Sprite::AddBound(RECT bound) {
	bounds.push_back(bound);
}

void Sprite::Draw() {
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

	//Why is it pixelated, i dunt undastand
	Game::GetInstance()->GetSpriteHandler()->Draw(
		texture,
		&bounds.at(currentFrame),
		nullptr,
		&position,
		D3DCOLOR_ARGB(255, 255, 255, 255)
	);
}

void Sprite::Release() {
	if (!texture) {
		texture->Release();
	}

	bounds.clear();
}
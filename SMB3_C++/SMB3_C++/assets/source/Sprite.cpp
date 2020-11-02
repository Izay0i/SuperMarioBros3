#include "../headers/Sprite.h"

void Sprite::LoadTexture() {
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

	position = D3DXVECTOR3(40, 0, 0);
	scale = D3DXVECTOR2(3, 3);
	D3DXMatrixTransformation2D(&matrix, &scale, 0, &scale, &rotation, 0, &translation);

	D3DXMATRIX oldMatrix;
	Game::GetInstance()->GetSpriteHandler()->GetTransform(&oldMatrix);
	Game::GetInstance()->GetSpriteHandler()->SetTransform(&matrix);

	//Why is it pixelated, i dunt undastand
	Game::GetInstance()->GetSpriteHandler()->Draw(
		texture,
		&bounds.at(currentFrame),
		nullptr,
		&position,
		D3DCOLOR_ARGB(255, 255, 255, 255)
	);

	Game::GetInstance()->GetSpriteHandler()->SetTransform(&oldMatrix);
}

void Sprite::Release() {
	if (!texture) {
		texture->Release();
	}

	bounds.clear();
}
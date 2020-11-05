#include "../headers/Tiles.h"

LPCWSTR Tiles::texturePath = nullptr;
LPDIRECT3DTEXTURE9 Tiles::texture = nullptr;
D3DCOLOR Tiles::colorKey = D3DCOLOR_XRGB(0, 0, 0);

void Tiles::LoadTexture(std::string path, D3DCOLOR color) {
	if (!texture) {
		texturePath = Util::ToLPCWSTR(path);
		colorKey = color;

		HRESULT hResult;
		D3DXIMAGE_INFO imageInfo;

		hResult = D3DXGetImageInfoFromFile(texturePath, &imageInfo);
		if (hResult != D3D_OK) {
			OutputDebugStringA("Failed to get image info\n");
			return;
		}

		hResult = D3DXCreateTextureFromFileEx(
			Game::GetInstance()->GetDevice(),
			texturePath,
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

void Tiles::AddHitBox(RECTF bound) {
	hitbox.AddHitBox(bound);
}

void Tiles::AddImage(RECT bound, D3DXVECTOR3 pos) {
	images.push_back(std::make_pair(bound, pos));
}

void Tiles::Render() {
	for (const auto& image : images) {
		float x = image.second.x - Camera::GetInstance()->GetPosition().x;
		float y = image.second.y - Camera::GetInstance()->GetPosition().y;
		D3DXVECTOR3 position = D3DXVECTOR3(x, y, 0);

		/*char debugStr[100];
		sprintf_s(debugStr, "Sprite position: %f %f\n", position.x, position.y);
		OutputDebugStringA(debugStr);*/

		Game::GetInstance()->GetSpriteHandler()->Draw(
			texture,
			&image.first,
			nullptr,
			&position,
			D3DCOLOR_XRGB(255, 255, 255)
		);
	}
}

void Tiles::Release() {
	if (texture) {
		texture->Release();
	}

	if (texturePath) {
		delete texturePath;
		texturePath = nullptr;
	}

	images.clear();
}
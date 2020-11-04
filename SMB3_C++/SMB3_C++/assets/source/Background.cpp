#include "../headers/Background.h"

LPCWSTR Background::texturePath = nullptr;
LPDIRECT3DTEXTURE9 Background::texture = nullptr;
D3DCOLOR Background::colorKey = D3DCOLOR_XRGB(0, 0, 0);

void Background::LoadTexture(std::string path, D3DCOLOR color) {
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

void Background::AddImage(RECT bound, D3DXVECTOR3 pos) {
	std::pair<RECT, D3DXVECTOR3> imageInfo = std::make_pair(bound, pos);
	
	images.push_back(imageInfo);
}

void Background::DrawBackground() {		
	for (const auto &image : images) {
		Game::GetInstance()->GetSpriteHandler()->Draw(
			texture,
			&image.first,
			nullptr,
			&image.second,
			D3DCOLOR_XRGB(255, 255, 255)
		);
	}
}

void Background::Release() {
	images.clear();
}
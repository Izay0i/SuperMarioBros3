#include "../headers/Background.h"

Background* Background::bgInstance = nullptr;
LPCWSTR Background::texturePath = nullptr;
LPDIRECT3DTEXTURE9 Background::texture = nullptr;
D3DCOLOR Background::colorKey = D3DCOLOR_XRGB(0, 0, 0);

Background::Background() {
	
}

Background* Background::GetInstance() {
	if (!bgInstance) {
		bgInstance = new Background;
	}

	return bgInstance;
}

void Background::LoadTexture(std::string path, D3DCOLOR color) {
	if (!texturePath) {
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
			directDevice,
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
			OutputDebugStringA("Failed to create background sprite from file\n");
			return;
		}
	}
}

void Background::AddImage(RECT bound, D3DXVECTOR3 pos) {	
	images.push_back(std::make_pair(bound, pos));
}

void Background::Render() {			
	for (const auto &image : images) {
		float x = image.second.x - Camera::GetInstance()->GetPosition().x;
		float y = image.second.y - Camera::GetInstance()->GetPosition().y;
		D3DXVECTOR2 spritePosition = D3DXVECTOR2(floor(x), floor(y));

		D3DXMATRIX mat;

		D3DXVECTOR2 scale(1.0f, 1.0f);
		D3DXMatrixTransformation2D(&mat, nullptr, 0.0f, &scale, nullptr, 0.0f, &spritePosition);
		spriteHandler->SetTransform(&mat);

		spriteHandler->Draw(
			texture,
			&image.first,
			nullptr,
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255)
		);
	}
}

void Background::Release() {	
	images.clear();
	
	if (texturePath) {
		delete texturePath;
		texturePath = nullptr;
	}

	if (texture) {
		texture->Release();
		texture = nullptr;
	}
}
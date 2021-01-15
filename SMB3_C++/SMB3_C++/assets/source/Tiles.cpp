#include "../headers/Tiles.h"

LPCWSTR Tiles::texturePath = nullptr;
LPDIRECT3DTEXTURE9 Tiles::texture = nullptr;
D3DCOLOR Tiles::colorKey = D3DCOLOR_XRGB(0, 0, 0);

Tiles::Tiles() {
	
}

Tiles::~Tiles() {}

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
			OutputDebugStringA("Failed to create texture from file\n");
			return;
		}
	}
}

void Tiles::AddHitBox(RECTF bound) {
	hitBox.AddHitBox(bound);
}

void Tiles::AddImage(RECT bound, D3DXVECTOR3 pos) {
	images.push_back(std::make_pair(bound, pos));
}

RECTF Tiles::GetBoundingBox(int id) const {
	RECTF bound;
	bound.left = position.x + 2;
	bound.top = position.y + 1;
	bound.right = position.x + hitBox.GetWidth(id) - 3;
	bound.bottom = position.y + hitBox.GetHeight(id) - 5;

	return bound;
}

void Tiles::Render() {
	for (const auto& image : images) {
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

void Tiles::Release() {
	images.clear();

	if (texture) {
		texture->Release();
		texture = nullptr;
	}

	if (texturePath) {
		delete texturePath;
		texturePath = nullptr;
	}
}
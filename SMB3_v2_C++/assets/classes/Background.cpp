#include "Background.h"

Background* Background::_backgroundInstance = nullptr;
LPDIRECT3DTEXTURE9 Background::_backgroundTexture = nullptr;

Background::Background() {}

Background::~Background() {}

Background* Background::GetInstance() {
	if (_backgroundInstance == nullptr) {
		_backgroundInstance = new Background;
	}
	return _backgroundInstance;
}

void Background::LoadTexture(std::string texturePath, D3DCOLOR colorKey) {
	if (_backgroundTexture != nullptr) {
		return;
	}

	_texturePath = GlobalUtil::ToLPCWSTR(texturePath);
	_colorKey = colorKey;

	HRESULT hResult;
	D3DXIMAGE_INFO imageInfo;
	hResult = D3DXGetImageInfoFromFile(_texturePath, &imageInfo);
	if (hResult != D3D_OK) {
		OutputDebugStringA("[BACKGROUND] Failed to get image info\n");
		return;
	}

	hResult = D3DXCreateTextureFromFileEx(
		GlobalUtil::directDevice,
		_texturePath,
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
		&_backgroundTexture
	);

	if (hResult != D3D_OK) {
		OutputDebugStringA("[BACKGROUND] Failed to create background sprite from file\n");
		return;
	}
}

void Background::AddSprite(RECT, D3DXVECTOR2) {

}

void Background::Render() {

}

void Background::Release() {

}
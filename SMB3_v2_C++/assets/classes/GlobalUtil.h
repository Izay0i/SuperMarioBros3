#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <vector>
#include <string>
#include <codecvt>

#include <d3d10.h>
#include <D3DX10.h>

#include <dinput.h>

#include <Windows.h>

namespace GlobalUtil {
	const unsigned int SCREEN_WIDTH = 256;
	const unsigned int SCREEN_HEIGHT = 224;
	//Used in all entity classes
	const unsigned int MAX_FILE_LINE = 5000;

	//Mainly used in the Game and Sprite class
	//Direct3D 10
	extern ID3D10Device* directDevice;
	extern ID3DX10Sprite* spriteHandler;
	//REMOVED
	//extern LPDIRECT3DDEVICE9 directDevice;
	//extern LPD3DXSPRITE spriteHandler;
	//END

	extern std::vector<std::string> SplitStr(std::string, std::string = "\t");

	extern std::vector<unsigned int> SplitDigit(unsigned int);

	extern LPCWSTR ToLPCWSTR(std::string);

	//https://stackoverflow.com/questions/26570721/how-to-know-if-wstring-can-be-safely-no-data-loss-converted-to-string
	extern std::string ToStr(std::wstring&);
}

typedef struct RectFloat {
	float left, top, right, bottom;

	RectFloat() : left(0), top(0), right(0), bottom(0) {}
	RectFloat(float l, float t, float r, float b) : left(l), top(t), right(r), bottom(b) {}
} RECTF;

//A structure that contains a loaded texture,
//and a shader resource view that is bound to it
struct Texture {
	unsigned int width = 0;
	unsigned int height = 0;

	ID3D10Texture2D* texture = nullptr;
	ID3D10ShaderResourceView* resourceView = nullptr;
	D3DXCOLOR colorKey = { 0, 0, 0, 0 };

	//Texture, shader resource view, color key/ color modulate
	Texture(ID3D10Texture2D* tex, ID3D10ShaderResourceView* srView, D3DXCOLOR key) {
		texture = tex;
		resourceView = srView;
		colorKey = key;

		D3D10_TEXTURE2D_DESC desc;
		texture->GetDesc(&desc);
		width = desc.Width;
		height = desc.Height;
	}
};
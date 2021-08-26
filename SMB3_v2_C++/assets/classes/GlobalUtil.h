#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <vector>
#include <string>
#include <codecvt>

#include <d3d9.h>
#include <d3dx9.h>

#include <dinput.h>

#include <Windows.h>

namespace GlobalUtil {
	const unsigned int SCREEN_WIDTH = 270;
	const unsigned int SCREEN_HEIGHT = 264;
	//NES aspect ratio
	const unsigned int ASPECT_RATIO_X = 8;
	const unsigned int ASPECT_RATIO_Y = 7;
	//Used in all entity classes
	const int MAX_FILE_LINE = 5000;

	extern unsigned int windowAdjustX, windowAdjustY;
	//Mainly used in the Game and Sprite class
	extern LPDIRECT3DDEVICE9 directDevice;
	extern LPD3DXSPRITE spriteHandler;

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
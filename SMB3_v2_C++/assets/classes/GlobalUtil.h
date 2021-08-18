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
	enum class KeyInput {
		KEY_INPUT_1 = DIK_1,
		KEY_INPUT_2 = DIK_2,
		KEY_INPUT_3 = DIK_3,
		KEY_INPUT_4 = DIK_4,
		KEY_INPUT_LEFT = DIK_A,
		KEY_INPUT_RIGHT = DIK_D,
		KEY_INPUT_UP = DIK_W,
		KEY_INPUT_DOWN = DIK_S,
		KEY_INPUT_SELECT = DIK_U,
		KEY_INPUT_START = DIK_I,
		KEY_INPUT_B = DIK_J,
		KEY_INPUT_A = DIK_K
	};

	const unsigned int SCREEN_WIDTH = 256;
	const unsigned int SCREEN_HEIGHT = 240;
	//NES aspect ratio
	const unsigned int ASPECT_RATIO_X = 8;
	const unsigned int ASPECT_RATIO_Y = 7;
	//Used in all entity classes
	const int MAX_FILE_LINE = 5000;

	extern unsigned int WINDOW_ADJUST_X, WINDOW_ADJUST_Y;

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
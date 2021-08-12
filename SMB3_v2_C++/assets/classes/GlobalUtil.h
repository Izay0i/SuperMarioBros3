#pragma once

#include <vector>
#include <string>
#include <codecvt>

#include <d3d9.h>
#include <d3dx9.h>

#include <Windows.h>

#define DIRECTINPUT_VERSION 0x0800

namespace GlobalUtil {
	const static unsigned int SCREEN_WIDTH = 263;
	const static unsigned int SCREEN_HEIGHT = 263;
	//Used in all entity classes
	const static int MAX_FILE_LINE = 5000;

	//Mainly used in the Game and Sprite class
	extern LPDIRECT3DDEVICE9 directDevice;
	extern LPD3DXSPRITE spriteHandler;

	static std::vector<std::string> SplitStr(std::string line, std::string delimeter = "\t") {
		std::vector<std::string> tokens;
		size_t last = 0, next = 0;

		while ((next = line.find(delimeter, last)) != std::string::npos) {
			tokens.push_back(line.substr(last, next - last));
			last = next + 1;
		}

		tokens.push_back(line.substr(last));

		return tokens;
	}

	static std::vector<unsigned int> SplitDigit(unsigned int number) {
		std::vector<unsigned int> digits;

		while (number > 0) {
			unsigned int digit = number % 10;
			number /= 10;
			digits.insert(digits.begin(), digit);
		}

		return digits;
	}

	static LPCWSTR ToLPCWSTR(std::string str) {
		const char* s = str.c_str();

		size_t newSize = strlen(s) + 1;
		wchar_t* wCString = new wchar_t[newSize];

		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wCString, newSize, s, _TRUNCATE);
		std::wstring* wStr = new std::wstring(wCString);

		return wStr->c_str();
	}

	//https://stackoverflow.com/questions/26570721/how-to-know-if-wstring-can-be-safely-no-data-loss-converted-to-string
	static std::string ToStr(std::wstring& wStr) {
		typedef std::codecvt_utf8<wchar_t> convertTypeX;
		std::wstring_convert<convertTypeX, wchar_t> converterX;

		return converterX.to_bytes(wStr);
	}
}

typedef struct RectFloat {
	float left, top, right, bottom;

	RectFloat() : left(0), top(0), right(0), bottom(0) {}
	RectFloat(float l, float t, float r, float b) : left(l), top(t), right(r), bottom(b) {}
} RECTF;
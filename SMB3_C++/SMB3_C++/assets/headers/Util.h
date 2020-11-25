#pragma once

#include <vector>
#include <string>
#include <codecvt>

#include <Windows.h>

namespace Util {
	static std::vector<std::string> split(std::string line, char delimeter = '\t') {
		std::vector<std::string> tokens;
		size_t last = 0, next = 0;

		while ((next = line.find(delimeter, last)) != std::string::npos) {
			tokens.push_back(line.substr(last, next - last));
			last = next + 1;
		}

		tokens.push_back(line.substr(last));

		return tokens;
	}

	static LPCWSTR ToLPCWSTR(std::string s) {
		const char* str = s.c_str();

		size_t newSize = strlen(str) + 1;
		wchar_t* wCString = new wchar_t[newSize];
		size_t convertedChars = 0;

		mbstowcs_s(&convertedChars, wCString, newSize, str, _TRUNCATE);
		std::wstring* wStr = new std::wstring(wCString);

		return wStr->c_str();
	}

	//https://stackoverflow.com/questions/26570721/how-to-know-if-wstring-can-be-safely-no-data-loss-converted-to-string
	static std::string ToStr(const std::wstring& wstr) {
		typedef std::codecvt_utf8<wchar_t> convertTypeX;
		std::wstring_convert<convertTypeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}
}

typedef struct RectFloat {
	float left;
	float top;
	float right;
	float bottom;

	RectFloat() : left(0), top(0), right(0), bottom(0) {}
	RectFloat(float l, float t, float r, float b) : left(l), top(t), right(r), bottom(b) {}
} RECTF;
#pragma once

#include <vector>
#include <string>

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
}
#pragma once

#include <vector>
#include <string>
#include <codecvt>

#include <Windows.h>

namespace Util {
	const static unsigned int SCREEN_WIDTH = 263;
	const static unsigned int SCREEN_HEIGHT = 263;

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

	static std::vector<unsigned int> splitDigit(unsigned int number) {
		std::vector<unsigned int> digits;
		
		while (number > 0) {
			unsigned int digit = number % 10;
			number /= 10;
			digits.insert(digits.begin(), digit);
		}

		return digits;
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

/*
Bury the light deep within
Cast aside there’s no coming home
We’re burning chaos in the wind
Drifting in the ocean all alone

Inherit the nightmare
Surrounded by fate
Can’t run away
Keep walking the line
Between the light
Led astray

Through vacant halls I won’t surrender
The truth revealed in eyes of ember
We fight through fire and ice forever
Two souls once lost and now they remember

I am the storm that is approaching
Provoking
Black clouds in isolation
I am reclaimer of my name
Born in flames
I have been blessed
My family crest is a demon of death

Forsakened I am awakened
A phoenix’s ash in dark divine
Descending misery
Destiny chasing time
Disappear into the night
Lost shadows left behind
Obsession’s pulling me
Fading I’ve come to take what’s mine

Lurking in the shadows under veil of night
Constellations of blood pirouette
Dancing through the graves of those who stand at my feet
Dreams of the black throne I keep on repeat
A derelict of dark summoned from the ashes
The puppet master congregates all the masses
Pulling strings twisting minds as blades hit
You want this power then come try and take it

Beyond the tree
Fire burns
Secret love
Bloodline yearns
Dark minds embrace
Crimson Joy
Does your dim heart
Heal or Destroy?

Bury the light deep within
Cast aside there’s no coming home
We’re burning chaos in the wind
Drifting in the ocean all alone
*/

//NOW I'M A LITTLE MOTIVATED
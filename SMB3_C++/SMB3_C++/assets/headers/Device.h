#pragma once

#include <dinput.h>
#include <d3d9.h>

class Device {
private:
	const static int KEYBOARD_BUFFER_SIZE = 1024;

	static Device* deviceInstance;

	LPDIRECTINPUT8 directInput;
	LPDIRECTINPUTDEVICE8 keyboard;

	BYTE keyStates[256];
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];

	Device();
	~Device();

public:
	static Device* GetInstance();

	bool InitKeyboard(HWND);

	void ProcessKeyboard();

	void Release();
};
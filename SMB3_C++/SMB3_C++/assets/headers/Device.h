#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <d3d9.h>

#include "SceneManager.h"

class Device {
private:
	const static int KEYBOARD_BUFFER_SIZE = 1024;

	static Device* deviceInstance;

	LPDIRECTINPUT8 directInput;
	LPDIRECTINPUTDEVICE8 keyboard;

	static BYTE keyStates[256];
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];

	Device();
	~Device();

public:
	static Device* GetInstance();

	bool InitKeyboard(HWND);

	static bool IsKeyDown(int keyCode);

	void ProcessKeyboard();

	void Release();
};
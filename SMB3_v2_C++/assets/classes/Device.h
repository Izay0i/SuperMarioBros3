#pragma once

#include "SceneManager.h"

class Device {
private:
	const static unsigned int _KEYBOARD_BUFFER_SIZE = 1024;

	static Device* _deviceInstance;

	SceneManager* _managerInstance;

	static BYTE _keyStates[256];
	DIDEVICEOBJECTDATA keyEvents[_KEYBOARD_BUFFER_SIZE];

	LPDIRECTINPUT8 _directInput;
	LPDIRECTINPUTDEVICE8 _keyboard;

	Device();
	~Device();

public:
	static Device* GetInstance();
	static bool IsKeyDown(int);

	bool InitKeyboard(HWND);
	void ProcessKeyboardInputs();
	void Release();
};
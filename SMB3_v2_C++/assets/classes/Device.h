#pragma once

#include "GlobalUtil.h"
#include "SceneManager.h"

#include <dinput.h>
#include <d3d9.h>

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
	enum class KeyInput {
		KEY_INPUT_LEFT = DIK_A,
		KEY_INPUT_RIGHT = DIK_D,
		KEY_INPUT_UP = DIK_W,
		KEY_INPUT_DOWN = DIK_S,
		KEY_INPUT_SELECT = DIK_U,
		KEY_INPUT_START = DIK_I,
		KEY_INPUT_B = DIK_J,
		KEY_INPUT_A = DIK_K
	};

	static Device* GetInstance();
	static bool IsKeyDown(int);

	bool InitKeyboard(HWND);
	void ProcessKeyboardInputs();
	void Release();
};
#pragma once

#include "SceneManager.h"

class Device {
private:
	enum class _DeviceFileSection {
		DEVICEFILE_SECTION_UNKNOWN,
		DEVICEFILE_SECTION_UP,
		DEVICEFILE_SECTION_DOWN,
		DEVICEFILE_SECTION_LEFT,
		DEVICEFILE_SECTION_RIGHT,
		DEVICEFILE_SECTION_SELECT,
		DEVICEFILE_SECTION_START,
		DEVICEFILE_SECTION_B,
		DEVICEFILE_SECTION_A
	};

	const static unsigned int _KEYBOARD_BUFFER_SIZE = 1024;

	static Device* _deviceInstance;

	std::unordered_map<std::string, int> _keysMap;
	std::unordered_map<std::string, int> _keyBinds;

	SceneManager* _managerInstance;

	static BYTE _keyStates[256];
	DIDEVICEOBJECTDATA keyEvents[_KEYBOARD_BUFFER_SIZE];

	LPDIRECTINPUT8 _directInput;
	LPDIRECTINPUTDEVICE8 _keyboard;

	int _GetKey(std::string) const;

	void _InitKeys();
	void _SetDefaultBinds();

	void _ParseKey(std::string, std::string);

	Device();
	~Device();

public:
	static Device* GetInstance();
	static bool IsKeyDown(int);

	bool InitKeyboard(HWND);

	int GetControllerKey(std::string);

	void LoadKeyBinds(std::string);

	void ProcessKeyboardInputs();
	void Release();
};
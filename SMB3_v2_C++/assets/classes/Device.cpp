#include "GlobalUtil.h"
#include "Device.h"

Device* Device::_deviceInstance = nullptr;
BYTE Device::_keyStates[256] = { ' ' };

int Device::_GetKey(std::string key) const {
	if (_keysMap.find(key) == _keysMap.end()) {
		return _keysMap.at("NULL");
	}
	return _keysMap.at(key);
}

void Device::_InitKeys() {
	//FUNCTION KEYS
	_keysMap.insert({ "F1", DIK_F1 });
	_keysMap.insert({ "F2", DIK_F2 });
	_keysMap.insert({ "F3", DIK_F3 });
	_keysMap.insert({ "F4", DIK_F4 });
	_keysMap.insert({ "F5", DIK_F5 });
	_keysMap.insert({ "F6", DIK_F6 });
	_keysMap.insert({ "F7", DIK_F7 });
	_keysMap.insert({ "F8", DIK_F8 });
	_keysMap.insert({ "F9", DIK_F9 });
	_keysMap.insert({ "F10", DIK_F10 });
	_keysMap.insert({ "F11", DIK_F11 });
	_keysMap.insert({ "F12", DIK_F12 });
	_keysMap.insert({ "F13", DIK_F13 });
	_keysMap.insert({ "F14", DIK_F14 });
	_keysMap.insert({ "F15", DIK_F15 });
	//NUMERIC KEYS
	_keysMap.insert({ "0", DIK_0 });
	_keysMap.insert({ "1", DIK_1 });
	_keysMap.insert({ "2", DIK_2 });
	_keysMap.insert({ "3", DIK_3 });
	_keysMap.insert({ "4", DIK_4 });
	_keysMap.insert({ "5", DIK_5 });
	_keysMap.insert({ "6", DIK_6 });
	_keysMap.insert({ "7", DIK_7 });
	_keysMap.insert({ "8", DIK_8 });
	_keysMap.insert({ "9", DIK_9 });
	//NUMPAD KEYS
	_keysMap.insert({ "NUMPAD0", DIK_NUMPAD0 });
	_keysMap.insert({ "NUMPAD1", DIK_NUMPAD1 });
	_keysMap.insert({ "NUMPAD2", DIK_NUMPAD2 });
	_keysMap.insert({ "NUMPAD3", DIK_NUMPAD3 });
	_keysMap.insert({ "NUMPAD4", DIK_NUMPAD4 });
	_keysMap.insert({ "NUMPAD5", DIK_NUMPAD5 });
	_keysMap.insert({ "NUMPAD6", DIK_NUMPAD6 });
	_keysMap.insert({ "NUMPAD7", DIK_NUMPAD7 });
	_keysMap.insert({ "NUMPAD8", DIK_NUMPAD8 });
	_keysMap.insert({ "NUMPAD9", DIK_NUMPAD9 });
	_keysMap.insert({ "NUMPADENTER", DIK_NUMPADENTER });
	_keysMap.insert({ "NUMLOCK", DIK_NUMLOCK });
	_keysMap.insert({ "NUMPADMUL", DIK_NUMPADSTAR });
	_keysMap.insert({ "NUMPADSUB", DIK_NUMPADMINUS });
	_keysMap.insert({ "NUMPADADD", DIK_NUMPADPLUS });
	_keysMap.insert({ "NUMPADDEC", DIK_NUMPADPERIOD });
	_keysMap.insert({ "NUMPADEQ", DIK_NUMPADEQUALS });
	_keysMap.insert({ "NUMPADDIV", DIK_NUMPADSLASH });
	//ALPHABET KEYS
	_keysMap.insert({ "A", DIK_A });
	_keysMap.insert({ "B", DIK_B });
	_keysMap.insert({ "C", DIK_C });
	_keysMap.insert({ "D", DIK_D });
	_keysMap.insert({ "E", DIK_E });
	_keysMap.insert({ "F", DIK_F });
	_keysMap.insert({ "G", DIK_G });
	_keysMap.insert({ "H", DIK_H });
	_keysMap.insert({ "I", DIK_I });
	_keysMap.insert({ "J", DIK_J });
	_keysMap.insert({ "K", DIK_K });
	_keysMap.insert({ "L", DIK_L });
	_keysMap.insert({ "M", DIK_M });
	_keysMap.insert({ "N", DIK_N });
	_keysMap.insert({ "O", DIK_O });
	_keysMap.insert({ "P", DIK_P });
	_keysMap.insert({ "Q", DIK_Q });
	_keysMap.insert({ "R", DIK_R });
	_keysMap.insert({ "S", DIK_S });
	_keysMap.insert({ "T", DIK_T });
	_keysMap.insert({ "U", DIK_U });
	_keysMap.insert({ "V", DIK_V });
	_keysMap.insert({ "W", DIK_W });
	_keysMap.insert({ "X", DIK_X });
	_keysMap.insert({ "Y", DIK_Y });
	_keysMap.insert({ "Z", DIK_Z });
	//CONTROL KEYS
	_keysMap.insert({ "ESC", DIK_ESCAPE });
	_keysMap.insert({ "TAB", DIK_TAB });
	_keysMap.insert({ "LSHIFT", DIK_LSHIFT });
	_keysMap.insert({ "RSHIFT", DIK_RSHIFT });
	_keysMap.insert({ "LCTRL", DIK_LCONTROL });
	_keysMap.insert({ "RCTRL", DIK_RCONTROL });
	_keysMap.insert({ "BACKSPACE", DIK_BACKSPACE });
	_keysMap.insert({ "ENTER", DIK_RETURN });
	_keysMap.insert({ "LALT", DIK_LALT });
	_keysMap.insert({ "RALT", DIK_RALT });
	_keysMap.insert({ "SPACE", DIK_SPACE });
	_keysMap.insert({ "CAPS", DIK_CAPSLOCK });
	_keysMap.insert({ "SCROLL", DIK_SCROLL });
	//SYMBOL KEYS
	_keysMap.insert({ "SUB", DIK_MINUS });
	_keysMap.insert({ "EQ", DIK_EQUALS });
	_keysMap.insert({ "LBRACKET", DIK_LBRACKET });
	_keysMap.insert({ "RBRACKET", DIK_RBRACKET });
	_keysMap.insert({ "SEMICOL", DIK_SEMICOLON });
	_keysMap.insert({ "APOS", DIK_APOSTROPHE });
	_keysMap.insert({ "GRAVE", DIK_GRAVE });
	_keysMap.insert({ "BACKSLASH", DIK_BACKSLASH });
	_keysMap.insert({ "COMMA", DIK_COMMA });
	_keysMap.insert({ "PERIOD", DIK_PERIOD });
	_keysMap.insert({ "SLASH", DIK_SLASH });
	//ARROW KEYPAD
	_keysMap.insert({ "HOME", DIK_HOME });
	_keysMap.insert({ "UP", DIK_UP });
	_keysMap.insert({ "PGUP", DIK_PGUP });
	_keysMap.insert({ "LEFT", DIK_LEFT });
	_keysMap.insert({ "RIGHT", DIK_RIGHT });
	_keysMap.insert({ "END", DIK_END });
	_keysMap.insert({ "DOWN", DIK_DOWN });
	_keysMap.insert({ "PGDN", DIK_PGDN });
	_keysMap.insert({ "INS", DIK_INSERT });
	_keysMap.insert({ "DEL", DIK_DELETE });
	//OTHER KEYS
	_keysMap.insert({ "LWIN", DIK_LWIN });
	_keysMap.insert({ "RWIN", DIK_RWIN });
	_keysMap.insert({ "NULL", 0x00 });
}

void Device::_SetDefaultBinds() {
	_keyBinds.insert({ "UP", _GetKey("W") });
	_keyBinds.insert({ "DOWN", _GetKey("S") });
	_keyBinds.insert({ "LEFT", _GetKey("A") });
	_keyBinds.insert({ "RIGHT", _GetKey("D") });
	_keyBinds.insert({ "SELECT", _GetKey("U") });
	_keyBinds.insert({ "START", _GetKey("I") });
	_keyBinds.insert({ "B", _GetKey("J") });
	_keyBinds.insert({ "A", _GetKey("K") });
}

void Device::_ParseKey(std::string input, std::string line) {
	if (line.empty()) {
		OutputDebugStringA("[DEVICE] Key is empty\n");
		return;
	}

	GlobalUtil::ToUpper(line);
	_keyBinds.at(input) = _GetKey(line);
}

Device::Device() {
	_InitKeys();
	_SetDefaultBinds();

	_managerInstance = SceneManager::GetInstance();
}

Device::~Device() {}

Device* Device::GetInstance() {
	if (_deviceInstance == nullptr) {
		_deviceInstance = new Device;
	}
	return _deviceInstance;
}

bool Device::IsKeyDown(int keyCode) {
	return (_keyStates[keyCode] & 0x80) > 0;
}

bool Device::InitKeyboard(HWND hWND) {
	HRESULT hResult;

	hResult = DirectInput8Create(
		reinterpret_cast<HINSTANCE>(GetWindowLong(hWND, GWL_HINSTANCE)),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		reinterpret_cast<VOID**>(&_directInput),
		nullptr
	);

	if (hResult != DI_OK) {
		MessageBoxA(hWND, "Failed to create input", "Error", MB_ICONERROR);
		return false;
	}

	hResult = _directInput->CreateDevice(GUID_SysKeyboard, &_keyboard, nullptr);

	if (hResult != DI_OK) {
		MessageBoxA(hWND, "Failed to create device in Device class", "Error", MB_ICONERROR);
		return false;
	}

	_keyboard->SetDataFormat(&c_dfDIKeyboard);
	_keyboard->SetCooperativeLevel(hWND, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = _KEYBOARD_BUFFER_SIZE;

	_keyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	hResult = _keyboard->Acquire();

	if (hResult != DI_OK) {
		MessageBoxA(hWND, "Failed to acquire keyboard", "Error", MB_ICONERROR);
		return false;
	}

	return true;
}

int Device::GetControllerKey(std::string key) {
	if (_keyBinds.find(key) == _keyBinds.end()) {
		return _GetKey(key);
	}
	return _keyBinds.at(key);
}

void Device::LoadKeyBinds(std::string filePath) {
	OutputDebugStringA("[DEVICE] Loading key binds\n");

	std::ifstream readFile;
	readFile.open(filePath, std::ios::in);

	if (!readFile.is_open()) {
		OutputDebugStringA("[DEVICE] Failed to read file\n");
		return;
	}

	_DeviceFileSection section = _DeviceFileSection::DEVICEFILE_SECTION_UNKNOWN;

	char str[GlobalUtil::MAX_FILE_LINE];
	while (readFile.getline(str, GlobalUtil::MAX_FILE_LINE)) {
		std::string line(str);

		if (line.empty() || line.front() == '#') {
			continue;
		}

		if (line == "[/]") {
			section = _DeviceFileSection::DEVICEFILE_SECTION_UNKNOWN;
			continue;
		}

		if (line == "[UP]") {
			section = _DeviceFileSection::DEVICEFILE_SECTION_UP;
			continue;
		}

		if (line == "[DOWN]") {
			section = _DeviceFileSection::DEVICEFILE_SECTION_DOWN;
			continue;
		}

		if (line == "[LEFT]") {
			section = _DeviceFileSection::DEVICEFILE_SECTION_LEFT;
			continue;
		}

		if (line == "[RIGHT]") {
			section = _DeviceFileSection::DEVICEFILE_SECTION_RIGHT;
			continue;
		}

		if (line == "[SELECT]") {
			section = _DeviceFileSection::DEVICEFILE_SECTION_SELECT;
			continue;
		}

		if (line == "[START]") {
			section = _DeviceFileSection::DEVICEFILE_SECTION_START;
			continue;
		}

		if (line == "[B]") {
			section = _DeviceFileSection::DEVICEFILE_SECTION_B;
			continue;
		}

		if (line == "[A]") {
			section = _DeviceFileSection::DEVICEFILE_SECTION_A;
			continue;
		}

		switch (section) {
			case _DeviceFileSection::DEVICEFILE_SECTION_UP:
				_ParseKey("UP", line);
				break;
			case _DeviceFileSection::DEVICEFILE_SECTION_DOWN:
				_ParseKey("DOWN", line);
				break;
			case _DeviceFileSection::DEVICEFILE_SECTION_LEFT:
				_ParseKey("LEFT", line);
				break;
			case _DeviceFileSection::DEVICEFILE_SECTION_RIGHT:
				_ParseKey("RIGHT", line);
				break;
			case _DeviceFileSection::DEVICEFILE_SECTION_SELECT:
				_ParseKey("SELECT", line);
				break;
			case _DeviceFileSection::DEVICEFILE_SECTION_START:
				_ParseKey("START", line);
				break;
			case _DeviceFileSection::DEVICEFILE_SECTION_B:
				_ParseKey("B", line);
				break;
			case _DeviceFileSection::DEVICEFILE_SECTION_A:
				_ParseKey("A", line);
				break;
		}
	}

	readFile.close();

	OutputDebugStringA("[DEVICE] Loaded key binds\n");
}

void Device::ProcessKeyboardInputs() {
	HRESULT hResult;
	hResult = _keyboard->GetDeviceState(sizeof(_keyStates), _keyStates);

	if (FAILED(hResult)) {
		if (hResult == DIERR_INPUTLOST || hResult == DIERR_NOTACQUIRED) {
			hResult = _keyboard->Acquire();

			if (hResult != DI_OK) {
				return;
			}
		}
		else {
			return;
		}
	}

	_managerInstance->GetCurrentScene()->HandleStates();

	DWORD elements = _KEYBOARD_BUFFER_SIZE;

	hResult = _keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), keyEvents, &elements, 0);

	if (FAILED(hResult)) {
		return;
	}

	for (DWORD i = 0; i < elements; ++i) {
		int keyCode = keyEvents[i].dwOfs;
		int keyState = keyEvents[i].dwData;

		if ((keyState & 0x80) > 0) {
			_managerInstance->GetCurrentScene()->OnKeyDown(keyCode);
		}
		else {
			_managerInstance->GetCurrentScene()->OnKeyUp(keyCode);
		}
	}
}

void Device::Release() {
	if (_deviceInstance != nullptr) {
		delete _deviceInstance;
		_deviceInstance = nullptr;
	}
}
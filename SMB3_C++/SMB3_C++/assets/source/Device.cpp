#include "../headers/Device.h"

Device* Device::deviceInstance = nullptr;
BYTE Device::keyStates[256] = { ' ' };

Device::Device() {}

Device::~Device() {
	deviceInstance->Release();

	if (directInput) {
		directInput->Release();
	}
}

Device* Device::GetInstance() {
	if (!deviceInstance) {
		deviceInstance = new Device;
	}
	return deviceInstance;
}

bool Device::InitKeyboard(HWND hWND) {
	HRESULT hResult;

	hResult = DirectInput8Create(
		reinterpret_cast<HINSTANCE>(GetWindowLong(hWND, GWL_HINSTANCE)),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		reinterpret_cast<VOID**>(&directInput),
		nullptr
	);

	if (hResult != DI_OK) {
		return false;
	}

	hResult = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, nullptr);

	if (hResult != DI_OK) {
		return false;
	}

	keyboard->SetDataFormat(&c_dfDIKeyboard);
	keyboard->SetCooperativeLevel(hWND, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE;

	keyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	hResult = keyboard->Acquire();

	if (hResult != DI_OK) {
		return false;
	}

	return true;
}

void Device::Release() {
	if (deviceInstance) {
		delete deviceInstance;
		deviceInstance = nullptr;
	}
}

bool Device::IsKeyDown(int keyCode) {
	return (keyStates[keyCode] & 0x80) > 0;
}

void Device::ProcessKeyboard() {
	HRESULT hResult;

	hResult = keyboard->GetDeviceState(sizeof(keyStates), keyStates);

	if (FAILED(hResult)) {
		if (hResult == DIERR_INPUTLOST || hResult == DIERR_NOTACQUIRED) {
			hResult = keyboard->Acquire();

			if (hResult != DI_OK) {
				return;
			}
		}
		else {
			return;
		}
	}

	SceneManager::GetInstance()->GetCurrentScene()->HandleStates(reinterpret_cast<BYTE*>(&keyStates));

	DWORD elements = KEYBOARD_BUFFER_SIZE;

	hResult = keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), keyEvents, &elements, 0);

	if (FAILED(hResult)) {
		return;
	}

	for (DWORD i = 0; i < elements; ++i) {
		int keyCode = keyEvents[i].dwOfs;
		int keyState = keyEvents[i].dwData;

		if ((keyState & 0x80) > 0) {
			//Scene::OnKeyDown(keyCode);
			SceneManager::GetInstance()->GetCurrentScene()->OnKeyDown(keyCode);
		}
		else {
			//Scene::OnKeyUp(keyCode);
			SceneManager::GetInstance()->GetCurrentScene()->OnKeyUp(keyCode);
		}
	}
}
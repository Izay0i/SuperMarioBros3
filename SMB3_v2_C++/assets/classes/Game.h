#pragma once

#include "GlobalUtil.h"
#include "Device.h"

#include <string>
#include <fstream>
#include <vector>

class Game {
private:
	enum class _GameFileSection {
		GAMEFILE_SECTION_UNKNOWN,
		GAMEFILE_SECTION_SETTINGS,
		GAMEFILE_SECTION_SCENES
	};

	const WCHAR* _CLASS_NAME = L"SMB3_Class";
	const WCHAR* _GAME_TITLE = L"Izay0i - Super Mario Bros 3";
	const WCHAR* _GAME_ICON = L"assets/smb3.ico";

	const static unsigned int _FRAME_RATE = 60;

	static Game* _gameInstance;

	int _defaultSceneID;
	bool _isRunning;

	Device* _deviceInstance;
	SceneManager* _managerInstance;

	HWND _hWND;
	
	LPDIRECT3D9 _direct3D;
	LPDIRECT3DSURFACE9 _backBuffer;

	static LRESULT CALLBACK _WinProc(HWND, UINT, WPARAM, LPARAM);
	static void _ResizeWindow(int, RECT&);

	Game();
	~Game();

	void _ParseSettings(std::string);
	void _Update(DWORD);
	void _Render();

public:
	static Game* GetInstance();

	HWND CreateGameWindow(HINSTANCE, int, int, int);

	bool InitGame(HWND);
	bool InitDevice();

	void LoadSettings(std::string);
	void GameRun();
};
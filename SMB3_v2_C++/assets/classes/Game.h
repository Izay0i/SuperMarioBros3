#pragma once

#include "Device.h"

#include <string>
#include <fstream>
#include <vector>

class Pipeline;
class Game {
private:
	enum class _GameFileSection {
		GAMEFILE_SECTION_UNKNOWN,
		GAMEFILE_SECTION_KEYBINDS,
		GAMEFILE_SECTION_SETTINGS,
		GAMEFILE_SECTION_SCENES
	};

	const WCHAR* _CLASS_NAME = L"SMB3_Class";
	const WCHAR* _GAME_TITLE = L"Izay0i - Super Mario Bros 3";
	const WCHAR* _GAME_ICON = L"assets/smb3.ico";

	//NES aspect ratio
	const static unsigned int _ASPECT_RATIO_X = 8;
	const static unsigned int _ASPECT_RATIO_Y = 7;

	const static unsigned int _FRAME_RATE = 60;

	static HWND _hWND;
	static HWND _contentHWND;

	static Game* _gameInstance;

	static LRESULT CALLBACK _WinProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK _WinProcContent(HWND, UINT, WPARAM, LPARAM);

	static void _ResizeWindow(int, RECT&);

	//NEW
	unsigned int _backBufferWidth;
	unsigned int _backBufferHeight;

	unsigned int _windowWidth;
	unsigned int _windowHeight;

	unsigned int _defaultSceneID;
	bool _isRunning;

	Device* _deviceInstance;
	SceneManager* _managerInstance;
	Pipeline* _pipelineInstance;

	Game();
	~Game();

	void _CreateContentWindow(HINSTANCE);

	void _ParseSettings(std::string);
	void _Update(DWORD);
	void _Render();

public:
	//Only changes on resize
	static unsigned int windowAdjustX;
	static unsigned int windowAdjustY;

	static Game* GetInstance();

	unsigned int GetBackBufferWidth() const;
	unsigned int GetBackBufferHeight() const;

	unsigned int GetWindowWidth() const;
	unsigned int GetWindowHeight() const;

	HWND CreateGameWindow(HINSTANCE, int, int, int);

	bool InitGame(HWND);
	bool InitDevice();

	void LoadSettings(std::string);
	void GameRun();
};
#pragma once

#include "string"
#include <fstream>
#include <vector>

#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

#include "Util.h"
#include "SceneManager.h"

class SceneManager;

class Game {
private:
	enum class FileSection {
		GAME_FILE_SECTION_UNKNOWN,
		GAME_FILE_SECTION_SETTINGS,
		GAME_FILE_SECTION_SCENES
	};

	const WCHAR* CLASS_NAME = L"SMB3_Class";
	const WCHAR* GAME_TITLE = L"Super Mario Bros 3";
	
	const static int MAX_FILE_LINE = 1024;
	const int FRAME_RATE = 60;

	static Game* gameInstance;
	
	int screenWidth, screenHeight;
	int currentSceneID;
	bool isRunning;

	SceneManager* sceneManager;

	HWND hWND;

	LPDIRECT3D9 direct3D;
	LPDIRECT3DDEVICE9 directDevice;
	
	LPDIRECT3DSURFACE9 backBuffer;

	LPD3DXSPRITE spriteHandler;
	
	static LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

	Game();
	~Game();

	void ParseSettings(std::string);
	
	void Update(DWORD);
	void Render();

public:
	static Game* GetInstance();

	int GetScreenWidth();
	int GetScreenHeight();

	LPDIRECT3DDEVICE9 GetDevice();
	LPD3DXSPRITE GetSpriteHandler();

	HWND CreateGameWindow(HINSTANCE, int, int, int);
	
	bool InitGame(HWND);
	bool InitDevice();
	
	void Load(std::string);
	void GameRun();
};
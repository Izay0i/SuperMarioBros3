#include "assets/classes/Game.h"

#include <Windows.h>

int __stdcall WinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nCmdShow) 
{
	Game* gameInstace = Game::GetInstance();

	HWND hWND = gameInstace->CreateGameWindow(hInstance, nCmdShow, GlobalUtil::SCREEN_WIDTH, GlobalUtil::SCREEN_HEIGHT);
	
	if (!gameInstace->InitGame(hWND)) {
		MessageBoxA(hWND, "Failed to create game", "Error", MB_ICONWARNING);
		return -1;
	}

	if (!gameInstace->InitDevice()) {
		MessageBoxA(hWND, "Failed to create device", "Error", MB_ICONWARNING);
		return -1;
	}

	gameInstace->LoadSettings("assets/data/settings.txt");
	SetWindowPos(hWND, 0, 0, 0, GlobalUtil::SCREEN_WIDTH * 3, GlobalUtil::SCREEN_HEIGHT * 3, SWP_NOOWNERZORDER | SWP_NOZORDER);
	gameInstace->GameRun();

	return 0;
}
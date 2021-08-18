#include "assets/classes/Game.h"

int __stdcall WinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nCmdShow) 
{
	RECT rect = { 0, 0, GlobalUtil::SCREEN_WIDTH, GlobalUtil::SCREEN_HEIGHT };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	GlobalUtil::WINDOW_ADJUST_X = (rect.right - rect.left) - GlobalUtil::SCREEN_WIDTH;
	GlobalUtil::WINDOW_ADJUST_Y = (rect.bottom - rect.top) - GlobalUtil::SCREEN_HEIGHT;

	Game* gameInstance = Game::GetInstance();

	HWND hWND = gameInstance->CreateGameWindow(hInstance, nCmdShow, GlobalUtil::SCREEN_WIDTH, GlobalUtil::SCREEN_HEIGHT);
	
	if (!gameInstance->InitGame(hWND)) {
		MessageBoxA(hWND, "Failed to create game", "Error", MB_ICONWARNING);
		return -1;
	}

	if (!gameInstance->InitDevice()) {
		MessageBoxA(hWND, "Failed to create device", "Error", MB_ICONWARNING);
		return -1;
	}

	gameInstance->LoadSettings("assets/data/settings.txt");
	SetWindowPos(hWND, 0, 0, 0, GlobalUtil::SCREEN_WIDTH * 3, GlobalUtil::SCREEN_HEIGHT * 3, SWP_NOOWNERZORDER | SWP_NOZORDER);
	gameInstance->GameRun();

	return 0;
}
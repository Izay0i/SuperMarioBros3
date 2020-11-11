//Let's see if I can make it in 5 days
//Guess not...
#include <Windows.h>

#include "assets/headers/Game.h"

//320	240
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Game* gameInstance = Game::GetInstance();

	HWND hWND = gameInstance->CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	if (!gameInstance->InitGame(hWND)) {
		MessageBoxA(hWND, "Failed to create game", "Error", MB_ICONWARNING);
		return -1;
	}

	if (!gameInstance->InitDevice()) {
		MessageBoxA(hWND, "Failed to create device", "Error", MB_ICONWARNING);
		return -1;
	}

	gameInstance->Load("assets/data/settings.txt");

	SetWindowPos(hWND, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	gameInstance->GameRun();

	/*char debugStr[100];
	for (int i = 1; i < 35; ++i) {
		sprintf_s(debugStr, "101	assets\\data\\tiles\\coin_data.txt\n");
		OutputDebugStringW(Util::ToLPCWSTR(debugStr));
	}*/

	return 0;
}
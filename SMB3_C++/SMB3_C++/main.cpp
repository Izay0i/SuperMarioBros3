#include <Windows.h>

#include "assets/headers/Game.h"

//271
const int SCREEN_WIDTH = 263;
const int SCREEN_HEIGHT = 263;

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

	//https://forums.codeguru.com/showthread.php?500841-Set-my-window-position-at-screen-center
	/*RECT rect;
	GetWindowRect(hWND, &rect);

	int xPos = (GetSystemMetrics(SM_CXSCREEN) - rect.right) / 2;
	int yPos = (GetSystemMetrics(SM_CYSCREEN) - rect.bottom) / 2;*/

	SetWindowPos(hWND, 0, 0, 0, SCREEN_WIDTH * 3, SCREEN_HEIGHT * 3, SWP_NOOWNERZORDER | SWP_NOZORDER);

	gameInstance->GameRun();

	/*char debugStr[100];
	for (int i = 0; i < 19; ++i) {
		for (int j = 0; j < 12; ++j) {
			sprintf_s(debugStr, "698	35	713	50	%d	%d	2\n", 2272 + 16 * i, 0 + 16 * j);
			OutputDebugStringA(debugStr);
		}
	}*/

	/*char debug[100];
	for (int i = 0; i < 12; ++i) {
		sprintf_s(debug, "698	86		713	101	%d	%d	2\n", 2256, 0 + 16 * i);
		OutputDebugStringA(debug);
	}*/

	return 0;
}
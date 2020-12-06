#include <Windows.h>

#include "assets/headers/Game.h"

const int SCREEN_WIDTH = 271;
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

	SetWindowPos(hWND, 0, 0, 0, SCREEN_WIDTH * 3, SCREEN_HEIGHT * 3, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	gameInstance->GameRun();

	/*char debugStr[100];
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 9; ++j) {
			sprintf_s(debugStr, "698	35	713	50	%d	%d	2\n", 2096 + 16 * i, 480 + 16 * j);
			OutputDebugStringA(debugStr);
		}
	}*/

	return 0;
}
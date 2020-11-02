//Let's see if I can make it in 5 days
//Guess not...

#include <Windows.h>

#include "assets/headers/Game.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

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

	SetWindowPos(hWND, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	gameInstance->GameRun();

	return 0;
}
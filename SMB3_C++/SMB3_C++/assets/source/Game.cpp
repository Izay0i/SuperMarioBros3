#include "../headers/Game.h"

LPDIRECT3DDEVICE9 Game::directDevice = nullptr;
LPD3DXSPRITE Game::spriteHandler = nullptr;

LRESULT CALLBACK Game::WinProc(HWND hWND, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_CHAR:
			switch (wParam) {
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
				case VK_RETURN:
					OutputDebugStringA("Enter key pressed\n");
					break;
			}
	}

	return DefWindowProc(hWND, message, wParam, lParam);
}

HWND Game::CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int width, int height) {	
	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = reinterpret_cast<WNDPROC>(WinProc);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hIcon = nullptr;
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hIconSm = nullptr;

	RegisterClassEx(&wndClass);

	HWND hWND = CreateWindow(
		CLASS_NAME,
		GAME_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (!hWND) {
		return FALSE;
	}

	ShowWindow(hWND, nCmdShow);
	UpdateWindow(hWND);

	return hWND;
}

Game* Game::gameInstance = nullptr;

Game::Game() {
	sceneManager = SceneManager::GetInstance();
}

Game::~Game() {
	if (sceneManager) {
		sceneManager->Release();
	}

	if (spriteHandler) {
		spriteHandler->Release();
	}

	if (backBuffer) {
		backBuffer->Release();
	}

	if (directDevice) {
		directDevice->Release();
	}

	if (direct3D) {
		direct3D->Release();
	}
}

void Game::ParseSettings(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 2) {
		return;
	}

	if (tokens.at(0) == "default") {
		currentSceneID = atoi(tokens.at(1).c_str());
	}
	else {
		OutputDebugStringA("Unknown game settings\n");
	}
}

void Game::Update(DWORD delta) {
	sceneManager->GetCurrentScene()->Update(delta);
}

void Game::Render() {
	//directDevice->Clear(0, nullptr, D3DCLEAR_TARGET, sceneManager->GetCurrentScene()->GetBGColor(), 0.0f, 0);

	if (directDevice->BeginScene()) {
		directDevice->ColorFill(backBuffer, nullptr, sceneManager->GetCurrentScene()->GetBGColor());

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
		
		//texture still bleeds even when set to point sampling
		directDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		directDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		directDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

		sceneManager->GetCurrentScene()->Render();

		spriteHandler->End();

		directDevice->EndScene();
	}

	directDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

Game* Game::GetInstance() {
	if (!gameInstance) {
		gameInstance = new Game;
	}
	return gameInstance;
}

int Game::GetScreenWidth() {
	return screenWidth;
}

int Game::GetScreenHeight() {
	return screenHeight;
}

LPDIRECT3DDEVICE9 Game::GetDevice() {
	return directDevice;
}

LPD3DXSPRITE Game::GetSpriteHandler() {
	return spriteHandler;
}

bool Game::InitGame(HWND hWND) {
	isRunning = true;
	
	direct3D = Direct3DCreate9(D3D_SDK_VERSION);

	this->hWND = hWND;

	D3DPRESENT_PARAMETERS directParams;

	ZeroMemory(&directParams, sizeof(directParams));

	directParams.Windowed = true;
	directParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	directParams.BackBufferFormat = D3DFMT_X8R8G8B8;
	directParams.BackBufferCount = 1;

	RECT window;
	GetClientRect(hWND, &window);

	directParams.BackBufferWidth = window.right + 1;
	directParams.BackBufferHeight = window.bottom + 1;

	screenWidth = window.right + 1;
	screenHeight = window.bottom + 1;

	direct3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWND,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&directParams,
		&directDevice
	);

	if (!directDevice) {
		return false;
	}

	directDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	HRESULT hResult = D3DXCreateSprite(directDevice, &spriteHandler);
	if (hResult != D3D_OK) {
		OutputDebugStringA("Failed to create sprite handler\n");
		return false;
	}

	sceneManager->SetDevice(directDevice);
	sceneManager->SetSpriteHandler(spriteHandler);

	return true;
}

bool Game::InitDevice() {
	if (!sceneManager->InitDevice(hWND)) {
		return false;
	}

	return true;
}

void Game::Load(std::string filePath) {
	std::ifstream readFile;
	readFile.open(filePath, std::ios::in);

	if (!readFile.is_open()) {
		OutputDebugStringA("Failed to read file\n");
		return;
	}

	FileSection section = FileSection::GAME_FILE_SECTION_UNKNOWN;

	char str[MAX_FILE_LINE];
	while (readFile.getline(str, MAX_FILE_LINE)) {
		std::string line(str);

		if (line[0] == '#' || line.empty()) {
			continue;
		}

		if (line == "[SETTINGS]") {
			section = FileSection::GAME_FILE_SECTION_SETTINGS;
			continue;
		}

		if (line == "[SCENES]") {
			section = FileSection::GAME_FILE_SECTION_SCENES;
			continue;
		}
		
		switch (section) {
			case FileSection::GAME_FILE_SECTION_SETTINGS:
				ParseSettings(line);
				break;
			case FileSection::GAME_FILE_SECTION_SCENES:
				sceneManager->ParseScenes(line);
				break;
		}
	}

	readFile.close();
	
	sceneManager->ChangeScene(currentSceneID);
}

void Game::GameRun() {
	MSG msg;
	DWORD frameStart = static_cast<DWORD>(GetTickCount64());
	DWORD tickPerFrame = 1000 / FRAME_RATE;

	while (isRunning) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				isRunning = false;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			DWORD now = static_cast<DWORD>(GetTickCount64());

			DWORD deltaTime = now - frameStart;

			if (deltaTime >= tickPerFrame) {
				frameStart = now;

				sceneManager->ProcessKeyboard();

				Update(deltaTime);
				Render();
			}
			else {
				Sleep(tickPerFrame - deltaTime);
			}
		}
	}
}
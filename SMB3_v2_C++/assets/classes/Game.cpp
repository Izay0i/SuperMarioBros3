#include "Game.h"

Game* Game::_gameInstance = nullptr;

LRESULT Game::_WinProc(HWND hWND, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_SIZING:
			_ResizeWindow(static_cast<int>(wParam), *reinterpret_cast<LPRECT>(lParam));
			break;
		case WM_GETMINMAXINFO:
			{
				MINMAXINFO* info = reinterpret_cast<MINMAXINFO*>(lParam);
				info->ptMinTrackSize.y = 
					((info->ptMinTrackSize.x - GlobalUtil::WINDOW_ADJUST_X) * 
					GlobalUtil::ASPECT_RATIO_Y) / GlobalUtil::ASPECT_RATIO_X + GlobalUtil::WINDOW_ADJUST_Y;
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_CHAR:
			switch (wParam) {
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
			}
	}

	return DefWindowProc(hWND, message, wParam, lParam);
}

//http://playtechs.blogspot.com/2007/10/forcing-window-to-maintain-particular.html
void Game::_ResizeWindow(int edge, RECT& rect) {
	int size_x_desired = (rect.right - rect.left) - GlobalUtil::WINDOW_ADJUST_X;
	int size_y_desired = (rect.bottom - rect.top) - GlobalUtil::WINDOW_ADJUST_Y;

	switch (edge)
	{
	case WMSZ_BOTTOM:
	case WMSZ_TOP:
	{
		int size_x = GlobalUtil::WINDOW_ADJUST_X + (size_y_desired * GlobalUtil::ASPECT_RATIO_X) / GlobalUtil::ASPECT_RATIO_Y;
		rect.left = (rect.left + rect.right) / 2 - size_x / 2;
		rect.right = rect.left + size_x;
	}
	break;
	case WMSZ_BOTTOMLEFT:
	{
		int size_x, size_y;

		if (size_x_desired * GlobalUtil::ASPECT_RATIO_Y > size_y_desired * GlobalUtil::ASPECT_RATIO_X)
		{
			size_x = rect.right - rect.left;
			size_y = GlobalUtil::WINDOW_ADJUST_Y + ((size_x - GlobalUtil::WINDOW_ADJUST_X) * GlobalUtil::ASPECT_RATIO_Y) / GlobalUtil::ASPECT_RATIO_X;
		}
		else
		{
			size_y = rect.bottom - rect.top;
			size_x = GlobalUtil::WINDOW_ADJUST_X + ((size_y - GlobalUtil::WINDOW_ADJUST_Y) * GlobalUtil::ASPECT_RATIO_X) / GlobalUtil::ASPECT_RATIO_Y;
		}

		rect.left = rect.right - size_x;
		rect.bottom = rect.top + size_y;
	}
	break;
	case WMSZ_BOTTOMRIGHT:
	{
		int size_x, size_y;

		if (size_x_desired * GlobalUtil::ASPECT_RATIO_Y > size_y_desired * GlobalUtil::ASPECT_RATIO_X)
		{
			size_x = rect.right - rect.left;
			size_y = GlobalUtil::WINDOW_ADJUST_Y + ((size_x - GlobalUtil::WINDOW_ADJUST_X) * GlobalUtil::ASPECT_RATIO_Y) / GlobalUtil::ASPECT_RATIO_X;
		}
		else
		{
			size_y = rect.bottom - rect.top;
			size_x = GlobalUtil::WINDOW_ADJUST_X + ((size_y - GlobalUtil::WINDOW_ADJUST_Y) * GlobalUtil::ASPECT_RATIO_X) / GlobalUtil::ASPECT_RATIO_Y;
		}

		rect.right = rect.left + size_x;
		rect.bottom = rect.top + size_y;
	}
	break;
	case WMSZ_LEFT:
	case WMSZ_RIGHT:
	{
		int size_y = GlobalUtil::WINDOW_ADJUST_Y + (size_x_desired * GlobalUtil::ASPECT_RATIO_Y) / GlobalUtil::ASPECT_RATIO_X;
		rect.top = (rect.top + rect.bottom) / 2 - size_y / 2;
		rect.bottom = rect.top + size_y;
	}
	break;
	case WMSZ_TOPLEFT:
	{
		int size_x, size_y;

		if (size_x_desired * GlobalUtil::ASPECT_RATIO_Y > size_y_desired * GlobalUtil::ASPECT_RATIO_X)
		{
			size_x = rect.right - rect.left;
			size_y = GlobalUtil::WINDOW_ADJUST_Y + ((size_x - GlobalUtil::WINDOW_ADJUST_X) * GlobalUtil::ASPECT_RATIO_Y) / GlobalUtil::ASPECT_RATIO_X;
		}
		else
		{
			size_y = rect.bottom - rect.top;
			size_x = GlobalUtil::WINDOW_ADJUST_X + ((size_y - GlobalUtil::WINDOW_ADJUST_Y) * GlobalUtil::ASPECT_RATIO_X) / GlobalUtil::ASPECT_RATIO_Y;
		}

		rect.left = rect.right - size_x;
		rect.top = rect.bottom - size_y;
	}
	break;
	case WMSZ_TOPRIGHT:
	{
		int size_x, size_y;

		if (size_x_desired * GlobalUtil::ASPECT_RATIO_Y > size_y_desired * GlobalUtil::ASPECT_RATIO_X)
		{
			size_x = rect.right - rect.left;
			size_y = GlobalUtil::WINDOW_ADJUST_Y + ((size_x - GlobalUtil::WINDOW_ADJUST_X) * GlobalUtil::ASPECT_RATIO_Y) / GlobalUtil::ASPECT_RATIO_X;
		}
		else
		{
			size_y = rect.bottom - rect.top;
			size_x = GlobalUtil::WINDOW_ADJUST_X + ((size_y - GlobalUtil::WINDOW_ADJUST_Y) * GlobalUtil::ASPECT_RATIO_X) / GlobalUtil::ASPECT_RATIO_Y;
		}

		rect.right = rect.left + size_x;
		rect.top = rect.bottom - size_y;
	}
	break;
	}
}

void Game::_ParseSettings(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 2) {
		return;
	}

	if (tokens.at(0) == "default") {
		_defaultSceneID = std::stoi(tokens.at(1));
	}
	else {
		OutputDebugStringA("[GAME] Unknown game settings\n");
	}
}

void Game::_Update(DWORD deltaTime) {
	_managerInstance->GetCurrentScene()->Update(deltaTime);
}

void Game::_Render() {
	if (GlobalUtil::directDevice->BeginScene() == D3D_OK) {
		GlobalUtil::directDevice->ColorFill(_backBuffer, nullptr, _managerInstance->GetCurrentScene()->GetBGColor());
		GlobalUtil::spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		_managerInstance->GetCurrentScene()->Render();
		
		GlobalUtil::spriteHandler->End();
		GlobalUtil::directDevice->EndScene();
	}
	GlobalUtil::directDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

Game::Game() {
	_deviceInstance = Device::GetInstance();
	_managerInstance = SceneManager::GetInstance();
}

Game::~Game() {
	if (_deviceInstance != nullptr) {
		_deviceInstance->Release();
	}

	if (_managerInstance != nullptr) {
		_managerInstance->Release();
	}

	if (GlobalUtil::spriteHandler != nullptr) {
		GlobalUtil::spriteHandler->Release();
	}

	if (_backBuffer != nullptr) {
		_backBuffer->Release();
	}

	if (GlobalUtil::directDevice != nullptr) {
		GlobalUtil::directDevice->Release();
	}

	if (_direct3D != nullptr) {
		_direct3D->Release();
	}
}

Game* Game::GetInstance() {
	if (_gameInstance == nullptr) {
		_gameInstance = new Game;
	}

	return _gameInstance;
}

HWND Game::CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int width, int height) {
	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = reinterpret_cast<WNDPROC>(_WinProc);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hIcon = reinterpret_cast<HICON>(LoadImage(hInstance, _GAME_ICON, IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
	wndClass.hIconSm = reinterpret_cast<HICON>(LoadImage(hInstance, _GAME_ICON, IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = _CLASS_NAME;

	RegisterClassEx(&wndClass);	

	HWND hWND = CreateWindow(
		_CLASS_NAME,
		_GAME_TITLE,
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

	if (hWND == nullptr) {
		return FALSE;
	}

	ShowWindow(hWND, nCmdShow);
	UpdateWindow(hWND);

	return hWND;
}

bool Game::InitGame(HWND hWND)
{
	_isRunning = true;

	_direct3D = Direct3DCreate9(D3D_SDK_VERSION);

	this->_hWND = hWND;

	D3DPRESENT_PARAMETERS directParams;

	ZeroMemory(&directParams, sizeof(directParams));

	directParams.Windowed = true;
	directParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	directParams.BackBufferFormat = D3DFMT_A8R8G8B8;
	directParams.BackBufferCount = 1;

	RECT window;
	GetClientRect(this->_hWND, &window);

	directParams.BackBufferWidth = window.right + 1;
	directParams.BackBufferHeight = window.bottom + 1;

	_direct3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		this->_hWND,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&directParams,
		&GlobalUtil::directDevice
	);

	if (GlobalUtil::directDevice == nullptr) {
		return false;
	}

	GlobalUtil::directDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &_backBuffer);

	HRESULT hResult = D3DXCreateSprite(GlobalUtil::directDevice, &GlobalUtil::spriteHandler);
	if (hResult != D3D_OK) {
		OutputDebugStringA("[GAME] Failed to create sprite handler\n");
		return false;
	}

	return true;
}

bool Game::InitDevice() {
	if (!_deviceInstance->InitKeyboard(_hWND)) {
		return false;
	}

	return true;
}

void Game::LoadSettings(std::string filePath) {
	std::ifstream readFile;
	readFile.open(filePath, std::ios::in);

	if (!readFile.is_open()) {
		OutputDebugStringA("[GAME] Failed to read file\n");
		return;
	}

	_GameFileSection section = _GameFileSection::GAMEFILE_SECTION_UNKNOWN;

	char str[GlobalUtil::MAX_FILE_LINE];
	while (readFile.getline(str, GlobalUtil::MAX_FILE_LINE)) {
		std::string line(str);

		if (line.empty() || line.front() == '#') {
			continue;
		}

		if (line == "[/]") {
			section = _GameFileSection::GAMEFILE_SECTION_UNKNOWN;
			continue;
		}

		if (line == "[SETTINGS]") {
			section = _GameFileSection::GAMEFILE_SECTION_SETTINGS;
			continue;
		}

		if (line == "[SCENES]") {
			section = _GameFileSection::GAMEFILE_SECTION_SCENES;
			continue;
		}

		switch (section) {
		case _GameFileSection::GAMEFILE_SECTION_SETTINGS:
			_ParseSettings(line);
			break;
		case _GameFileSection::GAMEFILE_SECTION_SCENES:
			_managerInstance->ParseScenes(line);
			break;
		}
	}

	readFile.close();

	_managerInstance->ChangeScene(_defaultSceneID);
}

void Game::GameRun() {
	MSG msg;
	DWORD frameStart = static_cast<DWORD>(GetTickCount64());
	DWORD ticksPerFrame = 1000 / _FRAME_RATE;

	while (_isRunning) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				_isRunning = false;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			DWORD now = static_cast<DWORD>(GetTickCount64());
			DWORD deltaTime = now - frameStart;

			if (deltaTime >= ticksPerFrame) {
				frameStart = now;

				_deviceInstance->ProcessKeyboardInputs();

				_Update(deltaTime);
				_Render();
			}
		}
	}
}
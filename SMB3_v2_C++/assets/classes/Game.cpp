#include "GlobalUtil.h"
#include "Game.h"
#include "Pipeline.h"
#include "audio/AudioService.h"
#include "audio/SFMLAudio.h"

HWND Game::_hWND = nullptr;
HWND Game::_contentHWND = nullptr;

Game* Game::_gameInstance = nullptr;

unsigned int Game::windowAdjustX = 0;
unsigned int Game::windowAdjustY = 0;

LRESULT Game::_WinProc(HWND hWND, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		//https://stackoverflow.com/questions/28968012/win32-add-black-borders-to-fullscreen-window
		//You have no idea how long it took just to find this
		case WM_SIZE:
			{
				const SIZE ar = { GlobalUtil::SCREEN_WIDTH, GlobalUtil::SCREEN_HEIGHT };
				//Query new client area size
				int clientWidth = LOWORD(lParam);
				int clientHeight = HIWORD(lParam);
				//Calculate new content size
				int contentWidth = MulDiv(clientHeight, ar.cx, ar.cy);
				int contentHeight = MulDiv(clientWidth, ar.cy, ar.cx);

				//Adjust dimensions to fit inside client area
				if (contentWidth > clientWidth) {
					contentWidth = clientWidth;
					contentHeight = MulDiv(contentWidth, ar.cy, ar.cx);
				}
				else {
					contentHeight = clientHeight;
					contentWidth = MulDiv(contentHeight, ar.cx, ar.cy);
				}

				//Calculate offsets to center content
				int offsetX = (clientWidth - contentWidth) / 2;
				int offsetY = (clientHeight - contentHeight) / 2;

				//Adjust content window position
				SetWindowPos(
					_contentHWND, 
					nullptr, 
					offsetX, 
					offsetY, 
					contentWidth, 
					contentHeight, 
					SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER
				);
			}
			break;
		case WM_SIZING:
			_ResizeWindow(static_cast<int>(wParam), *reinterpret_cast<LPRECT>(lParam));
			break;
		case WM_GETMINMAXINFO:
			{
				MINMAXINFO* info = reinterpret_cast<MINMAXINFO*>(lParam);
				info->ptMinTrackSize.y = 
					((info->ptMinTrackSize.x - Game::windowAdjustX) *
					_ASPECT_RATIO_Y) / _ASPECT_RATIO_X + Game::windowAdjustY;
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

LRESULT Game::_WinProcContent(HWND hWND, UINT message, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hWND, message, wParam, lParam);
}

//http://playtechs.blogspot.com/2007/10/forcing-window-to-maintain-particular.html
void Game::_ResizeWindow(int edge, RECT& rect) {
	int size_x_desired = (rect.right - rect.left) - windowAdjustX;
	int size_y_desired = (rect.bottom - rect.top) - windowAdjustY;

	switch (edge) {
		case WMSZ_BOTTOM:
		case WMSZ_TOP:
			{
				int size_x = windowAdjustX + (size_y_desired * _ASPECT_RATIO_X) / _ASPECT_RATIO_Y;
				rect.left = (rect.left + rect.right) / 2 - size_x / 2;
				rect.right = rect.left + size_x;
			}
			break;
		case WMSZ_BOTTOMLEFT:
			{
				int size_x, size_y;

				if (size_x_desired * _ASPECT_RATIO_Y > size_y_desired * _ASPECT_RATIO_X)
				{
					size_x = rect.right - rect.left;
					size_y = windowAdjustY + ((size_x - windowAdjustX) * _ASPECT_RATIO_Y) / _ASPECT_RATIO_X;
				}
				else
				{
					size_y = rect.bottom - rect.top;
					size_x = windowAdjustX + ((size_y - windowAdjustY) * _ASPECT_RATIO_X) / _ASPECT_RATIO_Y;
				}

				rect.left = rect.right - size_x;
				rect.bottom = rect.top + size_y;
			}
			break;
		case WMSZ_BOTTOMRIGHT:
			{
				int size_x, size_y;

				if (size_x_desired * _ASPECT_RATIO_Y > size_y_desired * _ASPECT_RATIO_X)
				{
					size_x = rect.right - rect.left;
					size_y = windowAdjustY + ((size_x - windowAdjustX) * _ASPECT_RATIO_Y) / _ASPECT_RATIO_X;
				}
				else
				{
					size_y = rect.bottom - rect.top;
					size_x = windowAdjustX + ((size_y - windowAdjustY) * _ASPECT_RATIO_X) / _ASPECT_RATIO_Y;
				}

				rect.right = rect.left + size_x;
				rect.bottom = rect.top + size_y;
			}
			break;
		case WMSZ_LEFT:
		case WMSZ_RIGHT:
			{
				int size_y = windowAdjustY + (size_x_desired * _ASPECT_RATIO_Y) / _ASPECT_RATIO_X;
				rect.top = (rect.top + rect.bottom) / 2 - size_y / 2;
				rect.bottom = rect.top + size_y;
			}
			break;
		case WMSZ_TOPLEFT:
			{
				int size_x, size_y;

				if (size_x_desired * _ASPECT_RATIO_Y > size_y_desired * _ASPECT_RATIO_X)
				{
					size_x = rect.right - rect.left;
					size_y = windowAdjustY + ((size_x - windowAdjustX) * _ASPECT_RATIO_Y) / _ASPECT_RATIO_X;
				}
				else
				{
					size_y = rect.bottom - rect.top;
					size_x = windowAdjustX + ((size_y - windowAdjustY) * _ASPECT_RATIO_X) / _ASPECT_RATIO_Y;
				}

				rect.left = rect.right - size_x;
				rect.top = rect.bottom - size_y;
			}
			break;
		case WMSZ_TOPRIGHT:
			{
				int size_x, size_y;

				if (size_x_desired * _ASPECT_RATIO_Y > size_y_desired * _ASPECT_RATIO_X)
				{
					size_x = rect.right - rect.left;
					size_y = windowAdjustY + ((size_x - windowAdjustX) * _ASPECT_RATIO_Y) / _ASPECT_RATIO_X;
				}
				else
				{
					size_y = rect.bottom - rect.top;
					size_x = windowAdjustX + ((size_y - windowAdjustY) * _ASPECT_RATIO_X) / _ASPECT_RATIO_Y;
				}

				rect.right = rect.left + size_x;
				rect.top = rect.bottom - size_y;
			}
			break;
	}
}

void Game::_CreateContentWindow(HINSTANCE hInstance) {
	WNDCLASSEX wndClass{ };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = reinterpret_cast<WNDPROC>(_WinProcContent);
	wndClass.hIcon = reinterpret_cast<HICON>(LoadImage(hInstance, _GAME_ICON, IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
	wndClass.hIconSm = reinterpret_cast<HICON>(LoadImage(hInstance, _GAME_ICON, IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wndClass.lpszClassName = L"Content_Class";
	RegisterClassEx(&wndClass);

	_contentHWND = CreateWindow(
		L"Content_Class",
		nullptr,
		WS_CHILD | WS_VISIBLE,
		0,
		0,
		0,
		0,
		_hWND,
		nullptr,
		hInstance,
		nullptr
	);

	if (_contentHWND == nullptr) {
		OutputDebugStringA("[GAME] _contentHWND was nullptr\n");
		return;
	}
}

void Game::_ParseSettings(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 2) {
		return;
	}

	if (tokens.at(0) == "default") {
		_defaultSceneID = std::stoul(tokens.at(1));
	}
	else {
		OutputDebugStringA("[GAME] Unknown game settings\n");
	}
}

void Game::_Update(DWORD deltaTime) {
	_managerInstance->GetCurrentScene()->Update(deltaTime);
}

void Game::_Render() {
	auto device = GlobalUtil::directDevice;
	auto spriteHandler = GlobalUtil::spriteHandler;
	auto currentScene = _managerInstance->GetCurrentScene();
	
	auto swapChain = _pipelineInstance->GetSwapChain();
	auto renderTargetView = _pipelineInstance->GetRenderTargetView();
	auto blendState = _pipelineInstance->GetBlendState();

	device->ClearRenderTargetView(renderTargetView, currentScene->GetBGColor());	
	spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE | D3DX10_SPRITE_SAVE_STATE);

	/*char debug[100];
	sprintf_s(debug, "D3DX_SDK_VERSION: %d", D3DX10_SDK_VERSION);
	RECT rect{};
	GlobalUtil::WriteTextToScreen(debug, &rect, DT_LEFT, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));*/

	//RGBA
	float newBlendFactor[4] = { 0.0f };
	device->OMSetBlendState(blendState, newBlendFactor, 0xffffffff);

	currentScene->Render();
	spriteHandler->End();
	swapChain->Present(0, 0);
}

Game::Game() {
	_deviceInstance = Device::GetInstance();
	_managerInstance = SceneManager::GetInstance();

	_pipelineInstance = Pipeline::GetInstance();
}

Game::~Game() {
	AudioService::GetAudio().Release();

	if (_pipelineInstance != nullptr) {
		_pipelineInstance->Release();
	}
	
	if (_deviceInstance != nullptr) {
		_deviceInstance->Release();
	}

	if (_managerInstance != nullptr) {
		_managerInstance->Release();
	}
}

Game* Game::GetInstance() {
	if (_gameInstance == nullptr) {
		_gameInstance = new Game;
	}
	return _gameInstance;
}

unsigned int Game::GetBackBufferWidth() const {
	return _backBufferWidth;
}

unsigned int Game::GetBackBufferHeight() const {
	return _backBufferHeight;
}

unsigned int Game::GetWindowWidth() const {
	return _windowWidth;
}

unsigned int Game::GetWindowHeight() const {
	return _windowHeight;
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
	wndClass.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = _CLASS_NAME;
	RegisterClassEx(&wndClass);	

	_hWND = CreateWindow(
		_CLASS_NAME,
		_GAME_TITLE,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (_hWND == nullptr) {
		OutputDebugStringA("[GAME] _hWND was nullptr\n");
		return FALSE;
	}

	_CreateContentWindow(hInstance);

	ShowWindow(_hWND, nCmdShow);
	UpdateWindow(_hWND);

	return _hWND;
}

bool Game::InitGame(HWND hWND) {
	_isRunning = true;

	RECT window;
	GetClientRect(hWND, &window);

	_backBufferWidth = window.right + 1;
	_backBufferHeight = window.bottom + 1;

	_windowWidth = window.right + 1;
	_windowHeight = window.bottom + 1;

	if (!_pipelineInstance->CreateDeviceAndSwapChain(_contentHWND, _backBufferWidth, _backBufferHeight, _FRAME_RATE)) {
		return false;
	}

	if (!_pipelineInstance->CreateRenderTagetView()) {
		return false;
	}

	//OPTIONAL:
	//If you want to flip sprites
	//Create a rasterizer state and disable culling
	//Source: https://gamedev.net/forums/topic/541543-cull-disable-dx10/4493656/
	if (!_pipelineInstance->CreateRasterizerState()) {
		return false;
	}

	if (!_pipelineInstance->CreateViewport(_backBufferWidth, _backBufferHeight)) {
		return false;
	}

	if (!_pipelineInstance->CreateBlendState()) {
		return false;
	}

	if (!_pipelineInstance->PipCreateFont("Time News Roman", 15, true, true)) {
		return false;
	}

	//Audio
	AudioService::SetAudio(new SFMLAudio);

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

				Sleep(0);
			}
			else {
				//Sleep(ticksPerFrame - deltaTime);
			}
		}
	}
}
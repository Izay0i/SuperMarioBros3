#include "Scene.h"

LPDIRECT3DTEXTURE9 Scene::_LoadTexture(LPDIRECT3DTEXTURE9 texture, LPCWSTR filePath, D3DCOLOR colorKey) {
	HRESULT hResult;
	D3DXIMAGE_INFO imageInfo;

	hResult = D3DXGetImageInfoFromFile(filePath, &imageInfo);
	if (hResult != D3D_OK) {
		OutputDebugStringA("[SCENE] Failed to get image info\n");
		return nullptr;
	}

	hResult = D3DXCreateTextureFromFileEx(
		GlobalUtil::directDevice,
		filePath,
		imageInfo.Width,
		imageInfo.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		colorKey,
		&imageInfo,
		nullptr,
		&texture
	);

	if (hResult != D3D_OK) {
		OutputDebugStringA("[SCENE] Failed to create background sprite from file\n");
		return nullptr;
	}

	return texture;
}

void Scene::_ParseSceneSize(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 2) {
		return;
	}

	_sceneWidth = std::stoul(tokens.at(0));
	_sceneHeight = std::stoul(tokens.at(1));
}

void Scene::_ParseSceneTime(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 1) {
		return;
	}

	_sceneTime = std::stoul(tokens.at(0));
}

void Scene::_ParseCameraBounds(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 4) {
		return;
	}

	RECTF cameraBound;
	cameraBound.left = std::stof(tokens.at(0));
	cameraBound.top = std::stof(tokens.at(1));
	cameraBound.right = std::stof(tokens.at(2));
	cameraBound.bottom = std::stof(tokens.at(3));

	_cameraInstance->AddCameraBound(cameraBound);
}

void Scene::_ParseBackgroundColor(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 3) {
		return;
	}

	unsigned int r = std::stoul(tokens.at(0));
	unsigned int g = std::stoul(tokens.at(1));
	unsigned int b = std::stoul(tokens.at(2));

	_backgroundColor = D3DCOLOR_XRGB(r, g, b);
}

void Scene::_ParseTextures(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 5) {
		return;
	}

	unsigned int textureID = std::stoul(tokens.at(0));

	unsigned int r = std::stoul(tokens.at(2));
	unsigned int g = std::stoul(tokens.at(3));
	unsigned int b = std::stoul(tokens.at(4));
	
	LPDIRECT3DTEXTURE9 texture = _LoadTexture(nullptr, GlobalUtil::ToLPCWSTR(tokens.at(1)), D3DCOLOR_XRGB(r, g, b));

	_textureMap.insert(std::make_pair(textureID, texture));
}

void Scene::_ParseEntityData(std::string) {

}

void Scene::_ParseTileData(std::string) {

}

void Scene::_ParseBackground(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() == 2) {
		unsigned int totalFrames = std::stoul(tokens.at(0));
		unsigned int textureID = std::stoul(tokens.at(1));
		_background = new Background(_textureMap[textureID], totalFrames);
		return;
	}

	if (tokens.size() < 6) {
		return;
	}

	RECT spriteBound;
	spriteBound.left = std::stoul(tokens.at(0));
	spriteBound.top = std::stoul(tokens.at(1));
	spriteBound.right = std::stoul(tokens.at(2));
	spriteBound.bottom = std::stoul(tokens.at(3));

	float x = std::stof(tokens.at(4));
	float y = std::stof(tokens.at(5));
	D3DXVECTOR2 position = D3DXVECTOR2(x, y);

	_background->AddSprite(spriteBound, position);
}

Scene::Scene(int sceneID, std::string path) {
	_sceneID = sceneID;
	_filePath = path;
	_toMapTime = 5000;
}

Scene::~Scene() {}

D3DCOLOR Scene::GetBGColor() const {
	return _backgroundColor;
}

void Scene::HandleStates(BYTE* states) {

}

void Scene::OnKeyUp(int keyCode) {

}

void Scene::OnKeyDown(int keyCode) {

}

void Scene::LoadScene() {
	char debug[100];
	sprintf_s(debug, "[SCENE] Loading scene ID: %d\n", _sceneID);
	OutputDebugStringA(debug);

	std::ifstream readFile;
	readFile.open(_filePath, std::ios::in);

	if (!readFile.is_open()) {
		OutputDebugStringA("[SCENE] Failed to read file\n");
		return;
	}

	//Load objects here, cause the Scene won't be calling destructor before the game ends
	_background = nullptr;
	_cameraInstance = Camera::GetInstance();
	//

	_SceneFileSection sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_UNKNOWN;

	char str[GlobalUtil::MAX_FILE_LINE];
	while (readFile.getline(str, GlobalUtil::MAX_FILE_LINE)) {
		std::string line(str);

		if (line.empty() || line.front() == '#') {
			continue;
		}

		if (line == "[/]") {
			sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_UNKNOWN;
			continue;
		}

		if (line == "[BGCOLOR]") {
			sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_BGCOLOR;
			continue;
		}

		if (line == "[TEXTURES]") {
			sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_TEXTURES;
			continue;
		}

		if (line == "[BACKGROUND]") {
			sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_BACKGROUND;
			continue;
		}

		switch (sceneFileSection) {
			case _SceneFileSection::SCENEFILE_SECTION_BGCOLOR:
				_ParseBackgroundColor(line);
				break;
			case _SceneFileSection::SCENEFILE_SECTION_TEXTURES:
				_ParseTextures(line);
				break;
			case _SceneFileSection::SCENEFILE_SECTION_BACKGROUND:
				_ParseBackground(line);
				break;
		}
	}

	readFile.close();
}

void Scene::Update(DWORD deltaTime) {

}

void Scene::Render() {
	if (_background != nullptr) {
		_background->Render();
	}
}

void Scene::Release() {
	if (_background != nullptr) {
		_background->Release();
		delete _background;
	}

	if (_cameraInstance != nullptr) {
		_cameraInstance->Release();
	}
	
	for (auto& activeEntity : _activeEntities) {
		activeEntity->Release();
		delete activeEntity;
	}
	_activeEntities.clear();

	for (auto& inactiveEntity : _inactiveEntities) {
		inactiveEntity->Release();
		delete inactiveEntity;
	}
	_inactiveEntities.clear();
	
	_textureMap.clear();
}
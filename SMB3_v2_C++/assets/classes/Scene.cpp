#include "Scene.h"

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
	std::ifstream readFile;
	readFile.open(_filePath, std::ios::in);

	if (!readFile.is_open()) {
		OutputDebugStringA("[SCENE] Failed to read file\n");
		return;
	}

	_SceneFileSection sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_UNKNOWN;

	char str[GlobalUtil::MAX_FILE_LINE];
	while (readFile.getline(str, GlobalUtil::MAX_FILE_LINE)) {
		std::string line(str);

		if (line.empty() || line.front() == '#') {
			continue;
		}
	}

	readFile.close();
}

void Scene::Update(DWORD deltaTime) {

}

void Scene::Render() {

}

void Scene::Release() {
	for (auto activeEntity : _activeEntities) {
		activeEntity->Release();
		delete activeEntity;
	}
	_activeEntities.clear();

	for (auto inactiveEntity : _inactiveEntities) {
		inactiveEntity->Release();
		delete inactiveEntity;
	}
	_inactiveEntities.clear();
}
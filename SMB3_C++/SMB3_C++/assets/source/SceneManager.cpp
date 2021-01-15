#include "../headers/SceneManager.h"

SceneManager* SceneManager::sceneManager = nullptr;

SceneManager::SceneManager() {
	deviceInput = Device::GetInstance();
}

SceneManager::~SceneManager() {
	if (deviceInput) {
		deviceInput->Release();
	}
}

SceneManager* SceneManager::GetInstance() {
	if (!sceneManager) {
		sceneManager = new SceneManager;
	}
	return sceneManager;
}

Scene* SceneManager::GetCurrentScene() {
	return scenes[currentSceneID];
}

void SceneManager::ParseScenes(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 2) {
		return;
	}

	int sceneID = std::stoi(tokens.at(0));
	std::string scenePath = tokens.at(1);
	
	scenes[sceneID] = new Scene(sceneID, scenePath);
}

void SceneManager::ChangeScene(unsigned int id) {
	scenes[currentSceneID]->Unload();

	currentSceneID = id;
	Scene* scene = scenes[currentSceneID];
	scene->Load(directDevice, spriteHandler);
}

void SceneManager::Release() {
	if (sceneManager) {
		delete sceneManager;
		sceneManager = nullptr;
	}
}

bool SceneManager::InitDevice(HWND hWND) {
	if (!deviceInput->InitKeyboard(hWND)) {
		return false;
	}

	return true;
}

void SceneManager::ProcessKeyboard() {
	deviceInput->ProcessKeyboard();
}
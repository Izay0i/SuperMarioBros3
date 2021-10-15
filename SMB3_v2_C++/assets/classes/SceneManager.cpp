#include "GlobalUtil.h"
#include "SceneManager.h"
#include "scene/SceneIntro.h"
#include "scene/SceneMap.h"
#include "scene/ScenePlay.h"

SceneManager* SceneManager::_managerInstance = nullptr;

SceneManager::SceneManager() {}

SceneManager::~SceneManager() {}

Scene* SceneManager::_CreateScene(unsigned int sceneID, std::string scenePath) {
	Scene::SceneType sceneType = static_cast<Scene::SceneType>(sceneID);
	switch (sceneType) {
		case Scene::SceneType::SCENE_TYPE_INTRO:
			return new SceneIntro(sceneType, scenePath);
		case Scene::SceneType::SCENE_TYPE_MAP:
			return new SceneMap(sceneType, scenePath);
		default:
			return new ScenePlay(sceneType, scenePath);
	}
}

SceneManager* SceneManager::GetInstance() {
	if (_managerInstance == nullptr) {
		_managerInstance = new SceneManager;
	}
	return _managerInstance;
}

Scene* SceneManager::GetCurrentScene() {
	return _scenes[_currentSceneID];
}

void SceneManager::ParseScenes(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 2) {
		return;
	}

	unsigned int sceneID = std::stoul(tokens.at(0));
	std::string scenePath = tokens.at(1);
	_scenes.insert(std::make_pair(sceneID, _CreateScene(sceneID, scenePath)));
}

void SceneManager::ChangeScene(unsigned int sceneID) {
	_scenes[_currentSceneID]->Release();
	_currentSceneID = sceneID;
	_scenes[_currentSceneID]->LoadScene();
}

void SceneManager::Release() {
	for (auto& scene : _scenes) {
		scene.second->Release();
		delete scene.second;
	}
	_scenes.clear();
	

	if (_managerInstance) {
		delete _managerInstance;
		_managerInstance = nullptr;
	}
}
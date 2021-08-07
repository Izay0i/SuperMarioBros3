#pragma once

#include "GlobalUtil.h"
#include "Scene.h"

#include <unordered_map>

class SceneManager {
private:
	static SceneManager* _managerInstance;

	int _currentSceneID;
	std::unordered_map<int, Scene*> _scenes;

	SceneManager();
	~SceneManager();

public:
	static SceneManager* GetInstance();

	Scene* GetCurrentScene();
	void ParseScenes(std::string);
	void ChangeScene(int);
	void Release();
};
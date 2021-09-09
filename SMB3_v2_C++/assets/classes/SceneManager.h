#pragma once

#include "Scene.h"

#include <unordered_map>

class SceneManager {
private:
	static SceneManager* _managerInstance;

	unsigned int _currentSceneID;
	std::unordered_map<unsigned int, Scene*> _scenes;

	SceneManager();
	~SceneManager();

public:
	static SceneManager* GetInstance();

	Scene* GetCurrentScene();
	void ParseScenes(std::string);
	void ChangeScene(unsigned int);
	void Release();
};
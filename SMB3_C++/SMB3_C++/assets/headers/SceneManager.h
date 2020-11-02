#pragma once

#include <unordered_map>

#include "Device.h"
#include "Scene.h"

class Device;
class Scene;

class SceneManager {
private:
	static SceneManager* sceneManager;

	int currentSceneID;

	std::unordered_map<unsigned int, Scene*> scenes;

	Device* deviceInput;

	SceneManager();
	~SceneManager();

public:
	static SceneManager* GetInstance();

	Scene* GetCurrentScene();

	void ParseScenes(std::string);

	void ChangeScene(unsigned int);
	void Release();

	bool InitDevice(HWND);
	void ProcessKeyboard();
};
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

	LPDIRECT3DDEVICE9 directDevice;
	LPD3DXSPRITE spriteHandler;

	SceneManager();
	~SceneManager();

public:
	static SceneManager* GetInstance();

	Scene* GetCurrentScene();

	void SetDevice(LPDIRECT3DDEVICE9& dev) { 
		if (!dev) {
			OutputDebugStringA("[SCENE MANAGER] Device is nulllptr\n");
		}
		directDevice = dev; 
	}
	LPDIRECT3DDEVICE9 GetDevice() { return directDevice; }

	void SetSpriteHandler(LPD3DXSPRITE& handler) { spriteHandler = handler; }
	LPD3DXSPRITE GetSpriteHandler() { return spriteHandler; }

	void ParseScenes(std::string);

	void ChangeScene(unsigned int);
	void Release();

	bool InitDevice(HWND);
	void ProcessKeyboard();
};
#pragma once

#include "GlobalUtil.h"
#include "spatial/Grid.h"
#include "GameObject.h"
#include "Camera.h"
#include "Background.h"
#include "Entity.h"
#include "player/Player.h"
#include "player/HUD.h"

#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>

class Scene {
private:
	enum class _SceneFileSection {
		SCENEFILE_SECTION_UNKNOWN,
		SCENEFILE_SECTION_SCENESIZE,
		SCENEFILE_SECTION_SCENETIME,
		SCENEFILE_SECTION_CAMERABOUNDS,
		SCENEFILE_SECTION_BGCOLOR,
		SCENEFILE_SECTION_TEXTURES,
		SCENEFILE_SECTION_ENTITYDATA,
		SCENEFILE_SECTION_TILEDATA,
		SCENEFILE_SECTION_HUD,
		SCENEFILE_SECTION_BACKGROUND
	};

	int _sceneID;
	unsigned int _sceneWidth, _sceneHeight;

	std::string _filePath;
	std::vector<Entity*> _entities;
	
	std::unordered_map<unsigned int, LPDIRECT3DTEXTURE9> _textureMap;

	D3DCOLOR _backgroundColor;
	DWORD _sceneTime;
	//Transition from scene to scene (5s)
	DWORD _toSceneStart;
	DWORD _toSceneTime;

	Camera* _cameraInstance;
	Background* _background;
	HUD* _hud;

	//Do the Mario, swing your arms from side to side, cmon let's go and do the Mario now
	Player* _mario;
	Player* _luigi;

	//Load textures and pass them to the game objects in the scene
	LPDIRECT3DTEXTURE9 _LoadTexture(LPDIRECT3DTEXTURE9, LPCWSTR, D3DCOLOR);

	void _ParseSceneSize(std::string);
	void _ParseSceneTime(std::string);
	void _ParseCameraBounds(std::string);
	void _ParseBackgroundColor(std::string);
	void _ParseTextures(std::string);
	void _ParseEntityData(std::string);
	void _ParseTileData(std::string);
	void _ParseHUD(std::string);
	void _ParseBackground(std::string);

public:
	enum class SceneType {
		SCENE_TYPE_INTRO = 0,
		SCENE_TYPE_MAP = 10,
		SCENE_TYPE_STAGE_ONE = 11,
		SCENE_TYPE_STAGE_FOUR = 14
	};

	Scene(int, std::string);
	~Scene();

	D3DCOLOR GetBGColor() const;

	void HandleStates(int, bool);
	void OnKeyUp(int);
	void OnKeyDown(int);

	void LoadScene();

	void Update(DWORD);
	void Render();

	void Release();
};
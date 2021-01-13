#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>

#include <dinput.h>

#include "Game.h"
#include "GameObject.h"
#include "Background.h"
#include "Entity.h"
#include "Camera.h"

#include "player/Mario.h"

#include "NPCList.h"
#include "TileList.h"

class Background;
class Mario;
class Camera;

class Scene {
private:
	enum class SceneSection {
		SCENE_FILE_SECTION_UNKNOWN,
		SCENE_FILE_SECTION_MAPSIZE,
		SCENE_FILE_SECTION_BGCOLOR,
		SCENE_FILE_SECTION_TEXTURES,
		SCENE_FILE_SECTION_ENTITYDATA,
		SCENE_FILE_SECTION_WORLDCOORDS,
		SCENE_FILE_SECTION_TILESDATA,
		SCENE_FILE_SECTION_TILESPRITES,
		SCENE_FILE_SECTION_BACKGROUND
	};

	enum class SceneType {
		SCENE_INTRO = 0,
		SCENE_MAP = 10,
		SCENE_STAGEONE = 11,
		SCENE_STAGEFOUR = 14
	};

	SceneType currentScene;

	Mario* marioInstance;
	Camera* cameraInstance;
	Background* bgInstance;

	const static int MAX_FILE_LINE = 2512;

	int sceneID;
	int sceneWidth, sceneHeight;

	std::string filePath;
	std::vector<GameObject*> objects;

	std::unordered_map<unsigned int, std::pair<std::string, D3DCOLOR>> textureFiles;

	D3DCOLOR backgroundColor;

	static LPDIRECT3DDEVICE9 directDevice;
	static LPD3DXSPRITE spriteHandler;

	~Scene();

	//https://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int
	bool IsInteger(const std::string&);

	void ParseMapSize(std::string);
	void ParseBGColor(std::string);
	void ParseTextures(std::string);
	void ParseEntityData(std::string);
	void ParseWorldCoords(std::string);
	void ParseTilesData(std::string);
	void ParseTileSprites(std::string);
	void ParseBackground(std::string);

	void UpdateCameraPosition();

public:
	Scene(int, std::string);

	D3DCOLOR GetBGColor() const;
	D3DCOLOR GetTextureColorKey(int) const;

	std::string GetTexturePath(int) const;

	int GetSceneID() const;

	int GetSceneWidth() const;
	int GetSceneHeight() const;	

	void AddObjectToScene(GameObject*);

	void Load(const LPDIRECT3DDEVICE9&, const LPD3DXSPRITE&);
	void Unload();

	void Update(DWORD);
	void Render();

	void HandleStates(BYTE*);
	void OnKeyDown(int);
	void OnKeyUp(int);
};
#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>

#include <dinput.h>

#include "Game.h"
#include "GameObject.h"
#include "Background.h"
#include "Tiles.h"
#include "Entity.h"
#include "Camera.h"

#include "player/Mario.h"

#include "npc/Goomba.h"

#include "tiles/Coin.h"
#include "tiles/QuestionBlock.h"
#include "tiles/ShinyBrick.h"
#include "tiles/BonusItem.h"

class Background;
class Mario;
class Camera;

class Scene {
private:
	enum class ObjectType {
		//entities
		OBJECT_TYPE_MARIO = 0,
		OBJECT_TYPE_GOOMBA = 1,
		OBJECT_TYPE_REDPARAGOOMBA = 2,
		OBJECT_TYPE_TROOPA = 3,
		OBJECT_TYPE_PARATROOPA = 4,
		OBJECT_TYPE_PIPLANT = 5,
		OBJECT_TYPE_VENUSTRAP = 6,
		//animated tiles
		OBJECT_TYPE_COIN = 101,
		OBJECT_TYPE_QUESTIONBLOCK = 102,
		OBJECT_TYPE_SHINYBRICK = 103,
		OBJECT_TYPE_BONUSITEM = 104,
	};

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

	Background* bgInstance;
	
	Mario* marioInstance;
	Camera* cameraInstance;

	const static int MAX_FILE_LINE = 2048;

	int sceneID;
	int sceneWidth, sceneHeight;

	std::string filePath;
	std::vector<GameObject*> objects;

	std::unordered_map<unsigned int, std::pair<std::string, D3DCOLOR>> textureFiles;

	D3DCOLOR backgroundColor;

	static LPDIRECT3DDEVICE9 directDevice;
	static LPD3DXSPRITE spriteHandler;

	~Scene();

	void ParseMapSize(std::string);
	void ParseBGColor(std::string);
	void ParseTextures(std::string);
	void ParseEntityData(std::string);
	void ParseWorldCoords(std::string);
	void ParseTilesData(std::string);
	void ParseTileSprites(std::string);
	void ParseBackground(std::string);

public:
	Scene(int, std::string);

	D3DCOLOR GetBGColor() const;
	D3DCOLOR GetTextureColorKey(int) const;

	std::string GetTexturePath(int) const;

	int GetSceneWidth() const;
	int GetSceneHeight() const;

	void Load(const LPDIRECT3DDEVICE9&, const LPD3DXSPRITE&);
	void Unload();

	void Update(DWORD);
	void Render();

	void HandleStates(BYTE* states);
	void OnKeyDown(int);
	void OnKeyUp(int);
};
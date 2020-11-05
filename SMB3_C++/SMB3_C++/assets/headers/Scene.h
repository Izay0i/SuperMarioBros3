#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>

#include <dinput.h>

//Welcome to a world of pain
//Something's wrong, I can feel it

#include "Background.h"
#include "Entity.h"
#include "Camera.h"

#include "player/Mario.h"

#include "npc/Goomba.h"
//#include "npc/Paragoomba"
//#include "npc/Microgoomba"
//#include "npc/RedParagoomba"?
//#include "npc/Troopa"
//#include "npc/Paratroopa"
//#include "npc/HammerBro"
//#include "npc/PiranaPlant"
//#include "npc/VenusTrap"

//#include "npc/Coin"
//#include "npc/SuperLeaf"
//#include "npc/SuperMushroom"

//tile or npc?
//#include "tile/SwitchBlock"
//#include "tile/QuestionBlock"
//#include "tile/ShinyBrick" //Destructable

//#include "tile/ColoredBlock"
//#include "tile/Sky"
//#include "tile/BlackSky"
//#include "tile/Cloud"
//#include "tile/Ground"
//#include "tile/Bush"
//#include "tile/TallBush"
//#include "tile/BlackBush"
//#include "tile/Brick"
//#include "tile/BlueBrick"
//#include "tile/Pipe"
//#include "tile/BlackPipe"

#define VK_A 0x41
#define VK_D 0x44
#define VK_F 0x46
#define VK_S 0x53
#define VK_W 0x57

class Game;
class Background;
class Mario;
class Camera;
class Entity;

class Scene {
private:
	enum class ObjectType {
		OBJECT_TYPE_MARIO = 0,
		OBJECT_TYPE_GOOMBA = 1,
		OBJECT_TYPE_REDPARAGOOMBA = 2,
		OBJECT_TYPE_TROOPA = 3,
		OBJECT_TYPE_PARATROOPA = 4,
		OBJECT_TYPE_PIPLANT = 5,
		OBJECT_TYPE_VENUSTRAP = 6
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

	const static int MAX_FILE_LINE = 1024;

	int sceneID;
	int sceneWidth, sceneHeight;

	std::string filePath;
	std::vector<Entity*> entities;

	std::unordered_map<unsigned int, std::pair<std::string, D3DCOLOR>> textureFiles;

	D3DCOLOR backgroundColor;

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

	void Load();
	void Unload();

	void Update(DWORD);
	void Render();

	void OnKeyDown(int);
	void OnKeyUp(int);
};
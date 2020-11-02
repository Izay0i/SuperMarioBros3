#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>

#include <dinput.h>

//Welcome to a world of pain
//Something's wrong, I can feel it

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

class Game;
class Mario;
class Camera;
class Entity;

class Scene {
private:
	enum class SceneSection {
		SCENE_FILE_SECTION_UNKNOWN,
		SCENE_FILE_SECTION_BGCOLOR,
		SCENE_FILE_SECTION_TEXTURES,
		SCENE_FILE_SECTION_ENTITYDATA,
		SCENE_FILE_SECTION_WORLDCOORDS
	};

	Mario* marioInstance;
	Camera* cameraInstance;

	const static int MAX_FILE_LINE = 1024;

	int sceneID;

	std::string filePath;
	std::vector<Entity*> entities;

	std::unordered_map<unsigned int, std::pair<std::string, D3DCOLOR>> textureFiles;

	D3DCOLOR backgroundColor;

	~Scene();

	void ParseBGColor(std::string);
	void ParseTextures(std::string);
	void ParseEntityData(std::string);
	void ParseWorldCoords(std::string);

public:
	Scene(int, std::string);

	D3DCOLOR GetBGColor();
	D3DCOLOR GetTextureColorKey(int);

	std::string GetTexturePath(int);

	void Load();
	void Unload();

	void Update(DWORD);
	void Render();

	static void OnKeyDown(int);
	static void OnKeyUp(int);
};
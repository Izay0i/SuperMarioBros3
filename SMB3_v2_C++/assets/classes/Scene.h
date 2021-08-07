#pragma once

#include "GlobalUtil.h"
#include "GameObject.h"
#include "Entity.h"

#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>

class Scene {
private:
	//TO-DO: redesign the parser for flexibility
	enum class _SceneFileSection {
		SCENEFILE_SECTION_UNKNOWN,
		SCENEFILE_SECTION_MAPSIZE,
		SCENEFILE_SECTION_CAMERABOUNDS,
		SCENEFILE_SECTION_MAPTIME,
		SCENEFILE_SECTION_BGCOLOR,
		SCENEFILE_SECTION_TEXTURES,
		SCENEFILE_SECTION_ENTITYDATA,
		SCENEFILE_SECTION_TILEDATA,
		SCENEFILE_SECTION_BACKGROUND
	};

	int _sceneID;
	int _sceneWidth, _sceneHeight;

	std::string _filePath;
	std::vector<Entity*> _activeEntities;
	std::vector<Entity*> _inactiveEntities;

	std::unordered_map<unsigned int, std::pair<std::string, D3DCOLOR>> _textureFiles;

	D3DCOLOR _backgroundColor;
	DWORD _sceneTime;
	//Transition from map to map (5s)
	DWORD _toMapStart;
	DWORD _toMapTime;

public:
	Scene(int, std::string);
	~Scene();

	D3DCOLOR GetBGColor() const;

	void HandleStates(BYTE*);
	void OnKeyUp(int);
	void OnKeyDown(int);

	void LoadScene();

	void Update(DWORD);
	void Render();

	void Release();
};
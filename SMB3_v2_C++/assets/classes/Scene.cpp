#include "GlobalUtil.h"
#include "Scene.h"
#include "NPCList.h"

bool Scene::_IsEntityInViewport(Entity* entity, RECTF viewport) const {
	float entityWidth = entity->GetPosition().x + entity->GetBoxWidth();
	float entityHeight = entity->GetPosition().y + entity->GetBoxHeight();
	if (entityWidth >= viewport.left && 
		entityHeight >= viewport.top && 
		entityWidth <= viewport.right && 
		entityHeight <= viewport.bottom) 
	{
		return true;
	}

	return false;
}

bool Scene::_IsEntityAliveOrIB(Entity* entity) const {
	if (entity->GetHealth() > -1) {
		return true;
	}
	
	float entityWidth = entity->GetPosition().x + entity->GetBoxWidth();
	float entityHeight = entity->GetPosition().y + entity->GetBoxHeight();
	if (entityWidth >= 0 && 
		entityHeight >= 0 && 
		entityWidth <= _sceneWidth && 
		entityHeight <= _sceneHeight) 
	{
		return true;
	}

	return false;
}

LPDIRECT3DTEXTURE9 Scene::_LoadTexture(LPDIRECT3DTEXTURE9 texture, LPCWSTR filePath, D3DCOLOR colorKey) {
	HRESULT hResult;
	D3DXIMAGE_INFO imageInfo;

	hResult = D3DXGetImageInfoFromFile(filePath, &imageInfo);
	if (hResult != D3D_OK) {
		OutputDebugStringA("[SCENE] Failed to get image info\n");
		return nullptr;
	}

	hResult = D3DXCreateTextureFromFileEx(
		GlobalUtil::directDevice,
		filePath,
		imageInfo.Width,
		imageInfo.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		colorKey,
		&imageInfo,
		nullptr,
		&texture
	);

	if (hResult != D3D_OK) {
		OutputDebugStringA("[SCENE] Failed to create background sprite from file\n");
		return nullptr;
	}

	return texture;
}

void Scene::_ParseSceneSize(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 2) {
		return;
	}

	_sceneWidth = std::stoul(tokens.at(0));
	_sceneHeight = std::stoul(tokens.at(1));
}

void Scene::_ParseSceneTime(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 1) {
		return;
	}

	_sceneTime = std::stoul(tokens.at(0));
}

void Scene::_ParseCameraBounds(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 4) {
		return;
	}

	RECTF cameraBound;
	cameraBound.left = std::stof(tokens.at(0));
	cameraBound.top = std::stof(tokens.at(1));
	cameraBound.right = std::stof(tokens.at(2));
	cameraBound.bottom = std::stof(tokens.at(3));

	_cameraInstance->AddCameraBound(cameraBound);
}

void Scene::_ParseBackgroundColor(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 3) {
		return;
	}

	unsigned int r = std::stoul(tokens.at(0));
	unsigned int g = std::stoul(tokens.at(1));
	unsigned int b = std::stoul(tokens.at(2));

	_backgroundColor = D3DCOLOR_XRGB(r, g, b);
}

void Scene::_ParseTextures(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 5) {
		return;
	}

	unsigned int textureID = std::stoul(tokens.at(0));

	unsigned int r = std::stoul(tokens.at(2));
	unsigned int g = std::stoul(tokens.at(3));
	unsigned int b = std::stoul(tokens.at(4));
	
	LPDIRECT3DTEXTURE9 texture = _LoadTexture(nullptr, GlobalUtil::ToLPCWSTR(tokens.at(1)), D3DCOLOR_XRGB(r, g, b));

	_textureMap.insert(std::make_pair(textureID, texture));
}

void Scene::_ParseEntityData(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 5) {
		return;
	}

	std::vector<std::string> extraData;
	if (tokens.size() > 5) {
		for (unsigned int i = 5; i < tokens.size(); ++i) {
			extraData.emplace_back(tokens.at(i));
		}
	}

	GameObject::GameObjectType objectType = static_cast<GameObject::GameObjectType>(
		std::stoul(
			tokens.at(0)
		)
	);

	float x = std::stof(tokens.at(3));
	float y = std::stof(tokens.at(4));
	D3DXVECTOR2 position = D3DXVECTOR2(x, y);

	unsigned int textureID = std::stoul(tokens.at(2));

	Entity* entity = nullptr;
	switch (objectType) {
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_MARIO:
			_mario = new Player;
			_mario->SetOjectType(objectType);
			_mario->ParseData(tokens.at(1), _textureMap[textureID], extraData);
			_mario->SetPosition(position);

			_entities.emplace_back(_mario);
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_LUIGI:
			_luigi = new Player;
			_luigi->SetOjectType(objectType);
			_luigi->ParseData(tokens.at(1), _textureMap[textureID], extraData);
			_luigi->SetPosition(position);

			_entities.emplace_back(_luigi);
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_GOOMBA:
			entity = new Goomba;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PARAGOOMBA:

			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_KOOPA:

			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PARAKOOPA:

			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PIRAPLANT:

			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_VENUSPLANT:

			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_BOOMERBRO:

			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PORTAL:

			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_MOVINGPLATFORM:

			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_COIN:

			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_BONUSITEM:

			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_QUESTIONBLOCK:

			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_SHINYBRICK:

			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PBLOCK:

			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_CACTUS:

			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_HELPTEXT:

			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_HAMMERBRO:

			break;
	}

	if (entity != nullptr) {
		entity->SetOjectType(objectType);
		entity->ParseData(tokens.at(1), _textureMap[textureID], extraData);
		entity->SetPosition(position);
		
		_entities.emplace_back(entity);
	}
}

void Scene::_ParseTileData(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 5) {
		return;
	}

	GameObject::GameObjectType objectType = static_cast<GameObject::GameObjectType>(std::stoul(tokens.at(0)));

	float x = std::stof(tokens.at(1));
	float y = std::stof(tokens.at(2));
	D3DXVECTOR2 position = D3DXVECTOR2(x, y);

	RECTF hitbox;
	hitbox.left = 0;
	hitbox.top = 0;
	hitbox.right = std::stof(tokens.at(3));
	hitbox.bottom = std::stof(tokens.at(4));
	
	Tile* tile = new Tile;
	tile->SetOjectType(objectType);
	tile->SetPosition(position);
	tile->AddHitbox(hitbox);
	
	_tiles.emplace_back(tile);
}

void Scene::_ParseGrid(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	_grid = new Grid(_sceneWidth, _sceneHeight);
	_grid->ParseData(line, _entities);
}

void Scene::_ParseHUD(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 2) {
		return;
	}

	unsigned int textureID = std::stoul(tokens.at(1));
	_hud = new HUD(_mario);
	_hud->ParseData(tokens.at(0), _textureMap[textureID]);
}

void Scene::_ParseBackground(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() == 2) {
		unsigned int totalFrames = std::stoul(tokens.at(0));
		unsigned int textureID = std::stoul(tokens.at(1));
		_background = new Background(_textureMap[textureID], totalFrames);
		return;
	}

	if (tokens.size() < 6) {
		return;
	}

	RECT spriteBound;
	spriteBound.left = std::stoul(tokens.at(0));
	spriteBound.top = std::stoul(tokens.at(1));
	spriteBound.right = std::stoul(tokens.at(2));
	spriteBound.bottom = std::stoul(tokens.at(3));

	float x = std::stof(tokens.at(4));
	float y = std::stof(tokens.at(5));
	D3DXVECTOR2 position = D3DXVECTOR2(x, y);

	_background->AddSprite(spriteBound, position);
}

Scene::Scene(SceneType sceneID, std::string path) {
	_sceneID = sceneID;
	_filePath = path;
	_toSceneTime = 5000;
}

Scene::~Scene() {}

D3DCOLOR Scene::GetBGColor() const {
	return _backgroundColor;
}

void Scene::HandleStates() {
	_mario->HandleStates();
}

void Scene::OnKeyUp(int keyCode) {
	_mario->OnKeyUp(keyCode);
}

void Scene::OnKeyDown(int keyCode) {
	_mario->OnKeyDown(keyCode);
}

void Scene::AddEntityToScene(Entity* entity) {
	_entities.emplace_back(entity);
	if (_grid != nullptr) {
		_grid->AddEntity(entity);
	}
}

void Scene::LoadScene() {
	char debug[100];
	sprintf_s(debug, "[SCENE] Loading scene with ID: %d\n", _sceneID);
	OutputDebugStringA(debug);

	std::ifstream readFile;
	readFile.open(_filePath, std::ios::in);

	if (!readFile.is_open()) {
		OutputDebugStringA("[SCENE] Failed to read file\n");
		return;
	}

	//Load objects here, cause the Scene won't be calling destructor before the game ends
	_entities.reserve(_MAX_ENTITIES_IN_SCENE);
	_tiles.reserve(_MAX_ENTITIES_IN_SCENE);

	_mario = nullptr;
	_luigi = nullptr;

	_hud = nullptr;
	_background = nullptr;
	_grid = nullptr;
	_cameraInstance = Camera::GetInstance();
	//

	_SceneFileSection sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_UNKNOWN;

	char str[GlobalUtil::MAX_FILE_LINE];
	while (readFile.getline(str, GlobalUtil::MAX_FILE_LINE)) {
		std::string line(str);

		if (line.empty() || line.front() == '#') {
			continue;
		}

		if (line == "[/]") {
			sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_UNKNOWN;
			continue;
		}

		if (line == "[SCENESIZE]") {
			sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_SCENESIZE;
			continue;
		}

		if (line == "[SCENETIME]") {
			sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_SCENETIME;
			continue;
		}

		if (line == "[CAMERABOUNDS]") {
			sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_CAMERABOUNDS;
			continue;
		}

		if (line == "[BGCOLOR]") {
			sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_BGCOLOR;
			continue;
		}

		if (line == "[TEXTURES]") {
			sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_TEXTURES;
			continue;
		}

		if (line == "[ENTITYDATA]") {
			sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_ENTITYDATA;
			continue;
		}

		if (line == "[TILEDATA]") {
			sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_TILEDATA;
			continue;
		}

		if (line == "[GRID]") {
			sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_GRID;
			continue;
		}

		if (line == "[HUD]") {
			sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_HUD;
			continue;
		}

		if (line == "[BACKGROUND]") {
			sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_BACKGROUND;
			continue;
		}

		switch (sceneFileSection) {
			case _SceneFileSection::SCENEFILE_SECTION_SCENESIZE:
				_ParseSceneSize(line);
				break;
			case _SceneFileSection::SCENEFILE_SECTION_SCENETIME:
				_ParseSceneTime(line);
				break;
			case _SceneFileSection::SCENEFILE_SECTION_CAMERABOUNDS:
				_ParseCameraBounds(line);
				break;
			case _SceneFileSection::SCENEFILE_SECTION_BGCOLOR:
				_ParseBackgroundColor(line);
				break;
			case _SceneFileSection::SCENEFILE_SECTION_TEXTURES:
				_ParseTextures(line);
				break;
			case _SceneFileSection::SCENEFILE_SECTION_ENTITYDATA:
				_ParseEntityData(line);
				break;
			case _SceneFileSection::SCENEFILE_SECTION_TILEDATA:
				_ParseTileData(line);
				break;
			case _SceneFileSection::SCENEFILE_SECTION_GRID:
				_ParseGrid(line);
				break;
			case _SceneFileSection::SCENEFILE_SECTION_HUD:
				_ParseHUD(line);
				break;
			case _SceneFileSection::SCENEFILE_SECTION_BACKGROUND:
				_ParseBackground(line);
				break;
		}
	}

	readFile.close();

	sprintf_s(debug, "[SCENE] Loaded scene with ID: %d\n", _sceneID);
	OutputDebugStringA(debug);
}

void Scene::UpdateCameraPosition() {
	unsigned int index = _mario->WentIntoPipe() ? 1 : 0;
	RECTF cameraBound = _cameraInstance->GetCameraBound(index);
	D3DXVECTOR2 cameraPosition = _cameraInstance->GetPosition();

	if (_mario->GetPosition().x < cameraPosition.x) {
		_mario->SetPosition({ cameraPosition.x, _mario->GetPosition().y });
	}
	else if (_mario->GetPosition().x + _mario->GetBoxWidth() > _sceneWidth) {
		_mario->SetPosition({ _sceneWidth - _mario->GetBoxWidth(), _mario->GetPosition().y });
	}

	switch (_sceneID) {
		case SceneType::SCENE_TYPE_STAGE_ONE:
			cameraPosition = _mario->GetPosition();
			//Bounds checking
			cameraPosition.x -= GlobalUtil::SCREEN_WIDTH / 2.5f;
			if (cameraPosition.x < cameraBound.left) {
				cameraPosition.x = cameraBound.left;
			}
			else if (cameraPosition.x + GlobalUtil::SCREEN_WIDTH > cameraBound.right) {
				cameraPosition.x = cameraBound.right - GlobalUtil::SCREEN_WIDTH;
			}

			cameraPosition.y -= GlobalUtil::SCREEN_HEIGHT / 2.5f;
			if (cameraPosition.y < cameraBound.top) {
				cameraPosition.y = cameraBound.top;
			}
			else if (cameraPosition.y + GlobalUtil::SCREEN_HEIGHT > cameraBound.bottom) {
				cameraPosition.y = cameraBound.bottom - GlobalUtil::SCREEN_HEIGHT;
			}
			break;
		case SceneType::SCENE_TYPE_STAGE_FOUR:
			
			break;
	}

	_cameraInstance->SetPosition(cameraPosition);
	/*char debug[100];
	sprintf_s(debug, "Camera position: %f %f\n", _cameraInstance->GetPosition().x, _cameraInstance->GetPosition().y);
	OutputDebugStringA(debug);
	RECTF viewport = _cameraInstance->GetViewport();
	sprintf_s(debug, "Viewport: %f %f %f %f\n", viewport.left, viewport.top, viewport.right, viewport.bottom);
	OutputDebugStringA(debug);*/
}

void Scene::Update(DWORD deltaTime) {
	/*if (_mario == nullptr && _luigi == nullptr) {
		char debug[100];
		sprintf_s(debug, "[SCENE] No player loaded in, scene ID: %d\n", _sceneID);
		OutputDebugStringA(debug);

		Sleep(5000);
		return;
	}*/
	
	std::sort(_entities.begin(), _entities.end(), Entity::CompareRenderPriority);
	switch (_sceneID) {
		case SceneType::SCENE_TYPE_INTRO:

			break;
		case SceneType::SCENE_TYPE_MAP:

			break;
		case SceneType::SCENE_TYPE_STAGE_ONE:
		case SceneType::SCENE_TYPE_STAGE_FOUR:
			if (_mario->GetHealth() > 0 && !_mario->TriggeredStageEnd()) {
				/*
				if (GetTickCount64() % 1000 == 0 && _sceneTime > 0) {
					--_sceneTime;
				}
				*/
			}

			if (_mario->GetHealth() > 0) {
				for (auto& entity : _entities) {
					entity->SetActive(_IsEntityInViewport(entity, _cameraInstance->GetViewport()));
					entity->Update(deltaTime, &_entities, &_tiles, _grid);

					if (_grid != nullptr) {
						Cell* newCell = _grid->GetCell(entity->GetPosition());
						if (newCell != entity->ownerCell) {
							_grid->RemoveEntityFromCell(entity);
							_grid->AddEntity(entity, newCell);
						}
					}

					if (!_IsEntityAliveOrIB(entity)) {
						if (_grid != nullptr) {
							_grid->RemoveEntityFromCell(entity);
						}

						entity->Release();
						delete entity;
						entity = nullptr;

						_entities.erase(std::remove(_entities.begin(), _entities.end(), entity), _entities.end());
					}
				}
			}

			if (_mario->TriggeredStageEnd() || _mario->GetHealth() == 0 || _sceneTime == 0) {
				//Warp back to map

			}
			break;
	}

	UpdateCameraPosition();
	if (_hud != nullptr) {
		_hud->Update(_sceneTime);
	}
}

void Scene::Render() {
	if (_background != nullptr) {
		_background->Render();
	}

	for (const auto& entity : _entities) {
		entity->Render();
	}

	if (_hud != nullptr) {
		_hud->Render();
	}
}

void Scene::Release() {
	char debug[100];
	sprintf_s(debug, "[SCENE] Unloading scene with ID: %d\n", _sceneID);
	OutputDebugStringA(debug);

	if (_hud != nullptr) {
		_hud->Release();
		delete _hud;
	}

	if (_background != nullptr) {
		_background->Release();
		delete _background;
	}

	if (_grid != nullptr) {
		_grid->Release();
		delete _grid;
	}

	if (_cameraInstance != nullptr) {
		_cameraInstance->Release();
	}
	
	for (auto& entity : _entities) {
		entity->Release();
		delete entity;
	}
	_entities.clear();
	
	for (auto& tile : _tiles) {
		tile->Release();
		delete tile;
	}
	_tiles.clear();

	for (auto& texture : _textureMap) {
		texture.second->Release();
	}
	_textureMap.clear();

	sprintf_s(debug, "[SCENE] Unloaded scene with ID: %d\n", _sceneID);
	OutputDebugStringA(debug);
}
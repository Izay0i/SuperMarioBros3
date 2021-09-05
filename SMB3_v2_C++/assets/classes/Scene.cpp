#include "GlobalUtil.h"
#include "Game.h"
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

bool Scene::_IsEntityAliveAndIB(Entity* entity) const {
	return true;
	
	if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_TAIL) {
		return  true;
	}

	float entityWidth = entity->GetPosition().x + entity->GetBoxWidth();
	float entityHeight = entity->GetPosition().y + entity->GetBoxHeight();
	if (entity->GetHealth() > -1 && (entityWidth >= 0 && 
		entityHeight >= 0 && 
		entityWidth <= _sceneWidth && 
		entityHeight <= _sceneHeight)) 
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

	GameObject::GameObjectType objectType = static_cast<GameObject::GameObjectType>(std::stoul(tokens.at(0)));

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
			//entity = new Goomba;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PARAGOOMBA:
			//entity = new Paragoomba;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_KOOPA:
			//entity = new Koopa;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PARAKOOPA:
			//entity = new Parakoopa;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PIRAPLANT:
			entity = new PiranaPlant;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_VENUSPLANT:
			//entity = new VenusPlant;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_BOOMERBRO:
			//entity = new BoomerBro;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PORTAL:
			//entity = new Portal;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_MOVINGPLATFORM:
			//entity = new MovingPlatform;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_COIN:
			//entity = new Coin;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_BONUSITEM:
			//entity = new BonusItem;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_QUESTIONBLOCK:
			//entity = new QuestionBlock;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_SHINYBRICK:
			//entity = new ShinyBrick;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PBLOCK:
			//entity = new PBlock;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_CACTUS:
			//entity = new Cactus;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_HELPTEXT:
			//entity = new HelpText;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_HAMMERBRO:
			//entity = new HammerBro;
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
	_grid = new Grid;
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

	if (tokens.size() == 1) {
		unsigned int textureID = std::stoul(tokens.at(0));
		_background = new Background(_textureMap[textureID]);
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

void Scene::RemoveEntityFromScene(Entity* entity) {
	if (_grid != nullptr) {
		_grid->RemoveEntityFromCell(entity);
	}
	_entities.erase(std::remove(_entities.begin(), _entities.end(), entity), _entities.end());
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
	const unsigned int MAX_ENTITIES_PER_SCENE = 200;
	_entities.reserve(MAX_ENTITIES_PER_SCENE);
	_tiles.reserve(MAX_ENTITIES_PER_SCENE);

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

	if (!_mario->TriggeredStageEnd() && !_mario->IsInPipe()) {
		if (_mario->GetPosition().x < cameraPosition.x) {
			_mario->SetPosition({ cameraPosition.x, _mario->GetPosition().y });
		}
		else if (_mario->GetPosition().x + _mario->GetBoxWidth() > _sceneWidth) {
			_mario->SetPosition({ _sceneWidth - _mario->GetBoxWidth(), _mario->GetPosition().y });
		}
	}

	cameraPosition = _mario->GetPosition();
	cameraPosition.x -= Game::GetInstance()->GetWindowWidth() / 2.25f;
	if (cameraPosition.x < cameraBound.left) {
		cameraPosition.x = cameraBound.left;
	}
	else if (cameraPosition.x + Game::GetInstance()->GetWindowWidth() > cameraBound.right) {
		cameraPosition.x = cameraBound.right - Game::GetInstance()->GetWindowWidth();
	}

	cameraPosition.y -= Game::GetInstance()->GetWindowHeight() / 2.25f;
	if (cameraPosition.y < cameraBound.top) {
		cameraPosition.y = cameraBound.top;
	}
	else if (cameraPosition.y + Game::GetInstance()->GetWindowHeight() > cameraBound.bottom) {
		cameraPosition.y = cameraBound.bottom - Game::GetInstance()->GetWindowHeight();
	}

	_cameraInstance->SetPosition(cameraPosition);
}

void Scene::Update(DWORD deltaTime) {
	if (_mario == nullptr && _luigi == nullptr) {
		char debug[100];
		sprintf_s(debug, "[SCENE] No player loaded in, scene ID: %d\n", _sceneID);
		OutputDebugStringA(debug);
		return;
	}
	
	std::sort(_entities.begin(), _entities.end(), Entity::CompareRenderPriority);
	switch (_sceneID) {
		case SceneType::SCENE_TYPE_INTRO:

			break;
		case SceneType::SCENE_TYPE_MAP:

			break;
		case SceneType::SCENE_TYPE_STAGE_ONE:
		case SceneType::SCENE_TYPE_STAGE_FOUR:
			if (_mario->GetHealth() > 0 && !_mario->TriggeredStageEnd()) {
				if (_sceneTime > 0 && GetTickCount64() % 1000 == 0) {
					--_sceneTime;
				}
			}

			if (_mario->GetHealth() > 0) {
				//Range-based loop, for_each, iterators will all be invalidated if an element is removed or inserted
				//And the container has to do a reallocation
				for (unsigned int i = 0; i < _entities.size(); ++i) {
					Entity* entity = _entities.at(i);
					entity->SetActive(_IsEntityInViewport(entity, _cameraInstance->GetViewport()));
					entity->Update(deltaTime, &_entities, &_tiles, _grid);

					if (_grid != nullptr) {
						Cell* newCell = _grid->GetCell(entity->GetPosition());
						if (newCell != entity->ownerCell) {
							_grid->RemoveEntityFromCell(entity);
							_grid->AddEntity(entity, newCell);
						}
					}

					if (!_IsEntityAliveAndIB(entity)) {
						if (_grid != nullptr) {
							_grid->RemoveEntityFromCell(entity);
						}
						delete entity;

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

	for (unsigned int i = 0; i < _entities.size(); ++i) {
		_entities.at(i)->Render();
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
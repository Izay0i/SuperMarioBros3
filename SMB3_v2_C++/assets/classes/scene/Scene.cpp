#include "../GlobalUtil.h"
#include "../Game.h"
#include "Scene.h"
#include "../EntityList.h"

bool Scene::_IsEntityInViewport(Entity* entity, RECTF viewport) const {
	//Ignore the player and tail, door and ceiling
	if (entity->GetObjectType() < GameObject::GameObjectType::GAMEOBJECT_TYPE_GOOMBA ||
		entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_TAIL ||
		entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_DOOR ||
		entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_MOVINGCEILING)
	{
		return  true;
	}

	float entityWidth = entity->GetPosition().x + entity->GetBoxWidth();
	float entityHeight = entity->GetPosition().y + entity->GetBoxHeight();
	if (entityWidth >= viewport.left && 
		entityHeight >= viewport.top && 
		entity->GetPosition().x <= viewport.right && 
		entity->GetPosition().y <= viewport.bottom) 
	{
		return true;
	}

	return false;
}

bool Scene::_IsEntityAliveAndIB(Entity* entity) const {
	//Ignore the player and tail, door and ceiling
	if (entity->GetObjectType() < GameObject::GameObjectType::GAMEOBJECT_TYPE_GOOMBA || 
		entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_TAIL || 
		entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_DOOR ||
		entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_MOVINGCEILING) 
	{
		return  true;
	}

	float entityWidth = entity->GetPosition().x + entity->GetBoxWidth();
	float entityHeight = entity->GetPosition().y + entity->GetBoxHeight();
	if (entity->GetHealth() > -1 && 
		(entityWidth >= 0 && 
		entityHeight >= 0 && 
		entity->GetPosition().x <= _sceneWidth && 
		entity->GetPosition().y <= _sceneHeight)) 
	{
		return true;
	}

	entity->flaggedForRemoval = true;
	return false;
}

unsigned int Scene::_GetNextThemeID() {
	//This wasn't the result I sought after
	//I needed something that could loop back to the first element when the container overflows
	/*auto it = std::find_if(
		_mainThemeIDs.begin(), 
		_mainThemeIDs.end(), 
		[&](unsigned int ID) {
			return ID == _currentThemeID;
		}
	);

	return *(std::next(it, 1));*/

	auto it = std::find(_mainThemeIDs.begin(), _mainThemeIDs.end(), _currentThemeID);
	++it;

	if (it >= _mainThemeIDs.end()) {
		it = _mainThemeIDs.begin();
	}

	_currentThemeID = *it;
	return _currentThemeID;
}

Texture* Scene::_LoadTexture(LPCWSTR filePath) {
	ID3D10Resource* resource = nullptr;
	ID3D10Texture2D* texture = nullptr;
	
	HRESULT hResult = D3DX10CreateTextureFromFile(
		GlobalUtil::directDevice, 
		filePath, 
		nullptr, 
		nullptr, 
		&resource, 
		nullptr
	);
	if (FAILED(hResult)) {
		OutputDebugStringA("[SCENE] Failed to create texture from file\n");
		return nullptr;
	}

	//Converts the ID3D10Resource into a ID3D10Texture2D object
	resource->QueryInterface(__uuidof(ID3D10Texture2D), reinterpret_cast<LPVOID*>(&texture));
	resource->Release();

	if (texture == nullptr) {
		OutputDebugStringA("[SCENE] Failed to convert the resource into a texture2D\n");
		return nullptr;
	}

	D3D10_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);

	D3D10_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
	ZeroMemory(&resourceViewDesc, sizeof(resourceViewDesc));
	resourceViewDesc.Format = desc.Format;
	resourceViewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MipLevels = desc.MipLevels;

	ID3D10ShaderResourceView* spriteTextureSRView = nullptr;
	GlobalUtil::directDevice->CreateShaderResourceView(texture, &resourceViewDesc, &spriteTextureSRView);

	return new Texture(texture, spriteTextureSRView);
}

void Scene::_ParseMainThemes(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 1) {
		return;
	}

	for (auto& token : tokens) {
		_mainThemeIDs.emplace_back(std::stoul(token));
	}
	_currentThemeID = _mainThemeIDs.front();
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

void Scene::_ParseCameraLockValue(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.empty()) {
		return;
	}

	_lockValue = std::stof(tokens.at(0));
}

void Scene::_ParseCameraBounds(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 5) {
		return;
	}

	RECTF cameraBound;
	cameraBound.left = std::stof(tokens.at(0));
	cameraBound.top = std::stof(tokens.at(1));
	cameraBound.right = std::stof(tokens.at(2));
	cameraBound.bottom = std::stof(tokens.at(3));

	float upVector = std::stof(tokens.at(4));
	_cameraInstance->AddUpVector(upVector);

	_cameraInstance->AddCameraBound(cameraBound);
}

void Scene::_ParseBackgroundColor(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 3) {
		return;
	}

	float r = std::stof(tokens.at(0)) / 255.0f;
	float g = std::stof(tokens.at(1)) / 255.0f;
	float b = std::stof(tokens.at(2)) / 255.0f;
	_backgroundColor = { r, g, b, 1.0f };
}

void Scene::_ParseTextures(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 2) {
		return;
	}

	unsigned int textureID = std::stoul(tokens.at(0));
	
	Texture* texture = _LoadTexture(GlobalUtil::ToLPCWSTR(tokens.at(1)));
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
	Texture* texture = GetTexture(textureID);

	Entity* entity = nullptr;
	switch (objectType) {
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_MARIO:
			_player = new Player;
			_player->SetOjectType(objectType);
			_player->ParseData(tokens.at(1), texture, extraData);
			_player->SetPosition(position);

			_entities.emplace_back(_player);
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_LUIGI:
			//Stub
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_GOOMBA:
			entity = new Goomba;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PARAGOOMBA:
			entity = new Paragoomba;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_KOOPA:
			entity = new Koopa;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PARAKOOPA:
			entity = new Parakoopa;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PIRANHAPLANT:
			entity = new PiranaPlant;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_VENUSPLANT:
			entity = new VenusPlant;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_BOOMERANGBRO:
			entity = new BoomerBro;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PODOBOO:
			entity = new Podoboo;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_DRYBONES:
			entity = new DryBones;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_ROTODISC:
			entity = new Rotodisc;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_TAIL:
			entity = new Tail;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PORTAL:
			entity = new Portal;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_MOVINGPLATFORM:
			//entity = new MovingPlatform;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_REDMUSHROOM:
			entity = new Mushroom;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_LEAF:
			entity = new Leaf;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_STAR:
			entity = new Star;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_COIN:
			entity = new Coin;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_BONUSITEM:
			entity = new BonusItem;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_ORB:
			entity = new Orb;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_QUESTIONBLOCK:
			entity = new QuestionBlock;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_SHINYBRICK:
			entity = new ShinyBrick;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PBLOCK:
			entity = new PBlock;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_LAVAPOOL:
			entity = new LavaPool;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_DOOR:
			entity = new Door;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_CACTUS:
			entity = new Cactus;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_HELPTEXT:
			entity = new HelpText;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_HAMMERBRO:
			entity = new HammerBro;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_DIALGA:
			entity = new Dialga;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PALKIA:
			entity = new Palkia;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_GIRATINA:
			entity = new Giratina;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_LOGO:
			entity = new GameLogo;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_ICON:
			entity = new GameIcon;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_SELECT:
			_selectText = new SelectText;
			_selectText->SetOjectType(objectType);
			_selectText->ParseData(tokens.at(1), texture, extraData);
			_selectText->SetPosition(position);
			
			_entities.emplace_back(_selectText);
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_CURTAIN:
			entity = new Curtain;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPPLANT:
			entity = new PropPlant;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_BUZZYBEETLE:
			entity = new BuzzyBeetle;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPMARIO:
			_propMario = new PropPlayer;
			_propMario->SetOjectType(objectType);
			_propMario->ParseData(tokens.at(1), texture, extraData);
			_propMario->SetPosition(position);

			_entities.emplace_back(_propMario);
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPLUIGI:
			_propLuigi = new PropPlayer;
			_propLuigi->SetOjectType(objectType);
			_propLuigi->ParseData(tokens.at(1), texture, extraData);
			_propLuigi->SetPosition(position);

			_entities.emplace_back(_propLuigi);
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPFASTKOOPA:
			entity = new PropFastKoopa;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPKOOPASHELL:
			entity = new PropKoopaShell;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPNORMALKOOPA:
			entity = new PropNormalKoopa;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_FORTRESSBOSS:
			entity = new FortressBoss;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_TRIGGER:
			entity = new Trigger;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_MASKTILE:
			entity = new MaskTile;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_MOVINGCEILING:
			entity = new MovingCeiling;
			break;
	}

	if (entity != nullptr) {
		entity->SetOjectType(objectType);
		entity->ParseData(tokens.at(1), texture, extraData);
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
	hitbox.left = -8.0f;
	hitbox.top = -8.0f;
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

void Scene::_ParseMainEffect(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 3) {
		return;
	}

	GameObject::GameObjectType objectType = static_cast<GameObject::GameObjectType>(std::stoul(tokens.at(0)));
	unsigned int textureID = std::stoul(tokens.at(2));
	Texture* texture = GetTexture(textureID);

	_scorePopUp = new ScorePopUp(_player);
	_scorePopUp->SetOjectType(objectType);
	_scorePopUp->ParseData(tokens.at(1), texture);
}

void Scene::_ParseHUD(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 2) {
		return;
	}

	unsigned int textureID = std::stoul(tokens.at(1));
	Texture* texture = GetTexture(textureID);

	_hud = new HUD(_player);
	_hud->ParseData(tokens.at(0), texture);
}

void Scene::_ParseBackground(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() == 1) {
		unsigned int textureID = std::stoul(tokens.at(0));
		Texture* texture = GetTexture(textureID);

		_background = new Background(texture);
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

bool Scene::IsTransitioningToScene() const {
	return _toSceneStart != 0;
}

void Scene::StartToSceneTimer() {
	_toSceneStart = static_cast<DWORD>(GetTickCount64());
}

Scene::SceneType Scene::GetSceneID() const {
	return _sceneID;
}

unsigned int Scene::GetSceneWidth() const {
	return _sceneWidth;
}

unsigned int Scene::GetSceneHeight() const {
	return _sceneHeight;
}

D3DXCOLOR Scene::GetBGColor() const {
	return _backgroundColor;
}

Texture* Scene::GetTexture(unsigned int id) const {
	if (_textureMap.find(id) == _textureMap.end()) {
		char debug[100];
		sprintf_s(debug, "[SCENE] No valid texture with the corresponding ID: %lu\n", id);
		OutputDebugStringA(debug);
		return nullptr;
	}

	return _textureMap.at(id);
}

void Scene::HandleStates() {}

void Scene::OnKeyUp(int keyCode) {}

void Scene::OnKeyDown(int keyCode) {}

void Scene::AddEntityToScene(Entity* entity) {
	_entities.emplace_back(entity);
	if (_grid != nullptr) {
		_grid->AddEntity(entity);
	}
}

void Scene::RemoveEntityFromScene(Entity* entity) {
	if (_grid != nullptr) {
		_grid->RemoveEntity(entity);
	}
	_entities.erase(std::remove(_entities.begin(), _entities.end(), entity), _entities.end());
}

Entity* Scene::CreateEntityFromData(std::string objectID, std::string dataPath, std::string textureID) {
	Entity* entity = nullptr;

	GameObject::GameObjectType objectType = static_cast<GameObject::GameObjectType>(std::stoul(objectID));
	unsigned int texID = std::stoul(textureID);
	Texture* texture = GetTexture(texID);

	switch (objectType) {
		//Projectiles
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PLAYERFIREBALL:
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_VENUSFIREBALL:
			entity = new Fireball;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_BOOMERANG:
			entity = new Boomerang;
			break;
		//Items
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_REDMUSHROOM:
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_GREENMUSHROOM:
			entity = new Mushroom;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_LEAF:
			entity = new Leaf;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_FLOWER:
			entity = new Flower;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_COIN:
			entity = new Coin;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_ORB:
			entity = new Orb;
			break;
		//Animated blocks
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_PBLOCK:
			entity = new PBlock;
			break;
		//Effects
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_BRICKEFFECT:
			entity = new BrickDebris;
			break;
		case GameObject::GameObjectType::GAMEOBJECT_TYPE_ORBEFFECT:
			entity = new OrbEffect;
			break;
	}

	entity->SetOjectType(objectType);
	entity->ParseData(dataPath, texture);
	return entity;
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
	const unsigned int MAX_ENTITIES_PER_SCENE = 256;
	_entities.reserve(MAX_ENTITIES_PER_SCENE);
	_tiles.reserve(MAX_ENTITIES_PER_SCENE);

	_player = nullptr;

	_propMario = nullptr;
	_propLuigi = nullptr;

	_selectText = nullptr;

	_scorePopUp = nullptr;
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

		if (line == "[MAINTHEMES]") {
			sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_MAINTHEMES;
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

		if (line == "[CAMERALOCKVALUE]") {
			sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_CAMERALOCKVALUE;
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

		if (line == "[MAINEFFECT]") {
			sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_MAINEFFECT;
			continue;
		}

		if (line == "[BACKGROUND]") {
			sceneFileSection = _SceneFileSection::SCENEFILE_SECTION_BACKGROUND;
			continue;
		}

		switch (sceneFileSection) {
			case _SceneFileSection::SCENEFILE_SECTION_MAINTHEMES:
				_ParseMainThemes(line);
				break;
			case _SceneFileSection::SCENEFILE_SECTION_SCENESIZE:
				_ParseSceneSize(line);
				break;
			case _SceneFileSection::SCENEFILE_SECTION_SCENETIME:
				_ParseSceneTime(line);
				break;
			case _SceneFileSection::SCENEFILE_SECTION_CAMERALOCKVALUE:
				_ParseCameraLockValue(line);
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
			case _SceneFileSection::SCENEFILE_SECTION_MAINEFFECT:
				_ParseMainEffect(line);
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

void Scene::UpdateCameraPosition() {}

void Scene::Update(DWORD deltaTime) {}

void Scene::Render() {}
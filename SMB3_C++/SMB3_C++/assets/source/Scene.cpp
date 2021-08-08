#include "../headers/Scene.h"

LPDIRECT3DDEVICE9 Scene::directDevice = nullptr;
LPD3DXSPRITE Scene::spriteHandler = nullptr;

Scene::Scene(int id, std::string path) {
	sceneID = id;
	filePath = path;
}

Scene::~Scene() {}

D3DCOLOR Scene::GetBGColor() const {
	return backgroundColor;
}

D3DCOLOR Scene::GetTextureColorKey(int id) const {
	return textureFiles.at(id).second;
}

std::string Scene::GetTexturePath(int id) const {
	return textureFiles.at(id).first;
}

int Scene::GetSceneID() const {
	return sceneID;
}

int Scene::GetSceneWidth() const {
	return sceneWidth;
}

int Scene::GetSceneHeight() const {
	return sceneHeight;
}

std::vector<GameObject*> Scene::GetActiveObjects() {
	/*std::vector<GameObject*> activeObjects = grid->GetActiveObjects();

	for (unsigned int i = 0; i < objects.size(); ++i) {
		if ((objects.at(i)->GetObjectID() < 201 && objects.at(i)->IsActive()) || objects.at(i)->GetObjectID() >= 201) {
			if (find(activeObjects.begin(), activeObjects.end(), objects.at(i)) == activeObjects.end()) {
				activeObjects.push_back(objects.at(i));
				
				char debug[100];
				sprintf_s(debug, "Active object ID: %d\n", activeObjects.at(i)->GetObjectID());
			}
		}
	}

	return activeObjects;*/

	std::vector<GameObject*> activeObjects;

	for (auto object : objects) {
		if ((object->GetPosition().x + object->GetBoxWidth() >= cameraInstance->GetViewport().left) &&
			(object->GetPosition().x <= cameraInstance->GetViewport().left + cameraInstance->GetViewport().right) &&
			(object->GetPosition().y + object->GetBoxHeight() >= cameraInstance->GetViewport().top) &&
			(object->GetPosition().y <= cameraInstance->GetViewport().top + cameraInstance->GetViewport().bottom))
		{
			if (find(activeObjects.begin(), activeObjects.end(), object) == activeObjects.end()) {
				//OutputDebugStringA("Active\n");
				activeObjects.push_back(object);
			}
		}
		else {
			if (find(disabledObjects.begin(), disabledObjects.end(), object) == disabledObjects.end()) {
				disabledObjects.push_back(object);
			}
		}
	}

	return activeObjects;
}

std::vector<GameObject*> Scene::GetDisabledObjects() {
	std::vector<GameObject*> disabledObjects;

	for (auto object : objects) {
		if ((object->GetPosition().x + object->GetBoxWidth() < cameraInstance->GetViewport().left) ||
			(object->GetPosition().x > cameraInstance->GetViewport().left + cameraInstance->GetViewport().right) ||
			(object->GetPosition().y + object->GetBoxHeight() < cameraInstance->GetViewport().top) ||
			(object->GetPosition().y > cameraInstance->GetViewport().top + cameraInstance->GetViewport().bottom)) 
		{
			if (find(disabledObjects.begin(), disabledObjects.end(), object) == disabledObjects.end()) {
				//OutputDebugStringA("Disabled\n");
				disabledObjects.push_back(object);
			}
		}
		else {
			if (find(activeObjects.begin(), activeObjects.end(), object) == activeObjects.end()) {
				activeObjects.push_back(object);
			}
		}
	}

	return disabledObjects;
}

DWORD Scene::GetSceneTime() const {
	return sceneTime;
}

void Scene::AddObjectToScene(GameObject* object) {
	if (object) {
		objects.push_back(object);
	}
	else {
		OutputDebugStringA("[SCENE] Inserted object is null\n");
	}
}

bool Scene::IsInteger(const std::string& s) {
	if (s.empty() || ((!isdigit(s.front())) && (s.front() != '-') && (s.front() != '+'))) {
		return false;
	}

	char* p;
	std::strtol(s.c_str(), &p, 10);

	return (*p == 0);
}

void Scene::ParseMapSize(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 2) {
		return;
	}

	sceneWidth = std::stoi(tokens.at(0));
	sceneHeight = std::stoi(tokens.at(1));
}

void Scene::ParseSceneTime(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 1) {
		return;
	}

	sceneTime = std::stoul(tokens.at(0));
}

void Scene::ParseCameraLimits(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 4) {
		return;
	}

	RECTF rect;
	rect.left = std::stof(tokens.at(0));
	rect.top = std::stof(tokens.at(1));
	rect.right = std::stof(tokens.at(2));
	rect.bottom = std::stof(tokens.at(3));

	cameraInstance->AddLimit(rect);
}

void Scene::ParseBGColor(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 3) {
		return;
	}

	unsigned int r = std::stoul(tokens.at(0));
	unsigned int g = std::stoul(tokens.at(1));
	unsigned int b = std::stoul(tokens.at(2));

	backgroundColor = D3DCOLOR_XRGB(r, g, b);
}

void Scene::ParseTextures(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 5) {
		return;
	}

	unsigned int texID = std::stoul(tokens.at(0));

	unsigned int r = std::stoul(tokens.at(2));
	unsigned int g = std::stoul(tokens.at(3));
	unsigned int b = std::stoul(tokens.at(4));

	textureFiles[texID] = std::pair<std::string, D3DCOLOR>(tokens.at(1), D3DCOLOR_XRGB(r, g, b));
}

void Scene::ParseHUD(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 2) {
		return;
	}

	int texID = std::stoi(tokens.at(1));
	hudInstance = HUD::GetInstance();
	hudInstance->ParseData(tokens.at(0), GetTexturePath(texID), GetTextureColorKey(texID));
}

void Scene::ParseGrid(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 1) {
		return;
	}

	std::string gridFilePath = tokens.at(0);

	std::ifstream inputFile;
	inputFile.open(gridFilePath, std::ios::in);

	if (!inputFile.is_open()) {
		OutputDebugStringA("[GRID] Failed to open grid file\n");
		return;
	}

	SceneSection section = SceneSection::SCENE_FILE_SECTION_UNKNOWN;

	char str[MAX_FILE_LINE];
	while (inputFile.getline(str, MAX_FILE_LINE)) {
		std::string line(str);

		if (line.empty() || line.front() == '#') {
			continue;
		}

		if (line == "[GRID_SIZE]") {
			section = SceneSection::SCENE_FILE_SECTION_GRID;
			continue;
		}

		if (line == "[ENTITY_DATA]") {
			section = SceneSection::SCENE_FILE_SECTION_ENTITYDATA;
			continue;
		}

		if (line == "[TILES_DATA]") {
			section = SceneSection::SCENE_FILE_SECTION_TILESDATA;
			continue;
		}

		switch (section) {
			case SceneSection::SCENE_FILE_SECTION_GRID:
				LoadGrid(line);
				break;
			case SceneSection::SCENE_FILE_SECTION_ENTITYDATA:
				LoadGridEntities(line);
				break;
			case SceneSection::SCENE_FILE_SECTION_TILESDATA:
				LoadGridTiles(line);
				break;
		}
	}

	inputFile.close();
}

void Scene::ParseEntityData(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 5) {
		return;
	}

	std::vector<std::string> extraData;

	if (tokens.size() > 5) {
		int objID = std::stoi(tokens.at(0));

		if (objID == 100) {
			for (unsigned int i = 5; i < tokens.size(); ++i) {
				extraData.push_back(tokens.at(i));
			}
		}
		else {
			//(variant) | objID | dataPath | texPath

			unsigned int begin = 5;
			//applies to bricks
			//objID | amount | dataPath ..
			if (objID == 103) {
				extraData.push_back(tokens.at(5));
				extraData.push_back(tokens.at(6));
				begin = 7;
			}

			for (unsigned int i = begin; i < tokens.size(); ++i) {
				if (IsInteger(tokens.at(i))) {
					extraData.push_back(GetTexturePath(std::stoi(tokens.at(i))));
				}
				else {
					extraData.push_back(tokens.at(i));
				}
			}
		}
	}
	
	float posX = std::stof(tokens.at(3));
	float posY = std::stof(tokens.at(4));
	D3DXVECTOR3 position = D3DXVECTOR3(posX, posY, 0);

	int texID = std::stoi(tokens.at(2));

	GameObject* object = nullptr;

	Entity::ObjectType objectID = static_cast<Entity::ObjectType>(std::stoi(tokens.at(0)));
	switch (objectID) {
		case Entity::ObjectType::OBJECT_TYPE_MARIO:
			marioInstance = Mario::GetInstance();			
			marioInstance->SetObjectID(static_cast<int>(objectID));
			marioInstance->ParseData(tokens.at(1), GetTexturePath(texID), GetTextureColorKey(texID), extraData);
			marioInstance->SetPosition(position);
			
			/*if (grid) {
				grid->InitObjects(marioInstance);
			}*/
			break;
		case Entity::ObjectType::OBJECT_TYPE_GOOMBA:
			object = new Goomba;
			break;
		case Entity::ObjectType::OBJECT_TYPE_PARAGOOMBA:
			object = new Paragoomba;
			break;
		case Entity::ObjectType::OBJECT_TYPE_TROOPA:
			object = new KoopaTroopa;
			break;
		case Entity::ObjectType::OBJECT_TYPE_PARATROOPA:
			object = new Parakoopa;
			break;
		case Entity::ObjectType::OBJECT_TYPE_PIPLANT:
			object = new PiranaPlant;
			break;
		case Entity::ObjectType::OBJECT_TYPE_VENUSTRAP:
			object = new VenusFire;
			break;
		case Entity::ObjectType::OBJECT_TYPE_BOOMERBRO:
			object = new BoomerBro;
			break;
		case Entity::ObjectType::OBJECT_TYPE_PORTAL:
			object = new Portal;
			break;
		case Entity::ObjectType::OBJECT_TYPE_COIN:
			object = new Coin;
			break;
		case Entity::ObjectType::OBJECT_TYPE_QUESTIONBLOCK:
			object = new QuestionBlock;
			break;
		case Entity::ObjectType::OBJECT_TYPE_SHINYBRICK:
			object = new ShinyBrick;
			break;
		case Entity::ObjectType::OBJECT_TYPE_BONUSITEM:
			object = new BonusItem;			
			break;
		case Entity::ObjectType::OBJECT_TYPE_SWITCHBLOCK:
			object = new SwitchBlock;
			break;
		case Entity::ObjectType::OBJECT_TYPE_CACTUS:
			object = new Cactus;
			break;
		case Entity::ObjectType::OBJECT_TYPE_HELP:
			object = new HelpPopUp;
			break;
		case Entity::ObjectType::OBJECT_TYPE_HAMMERBRONODE:
			object = new HammerBro;
			break;
		case Entity::ObjectType::OBJECT_TYPE_MOVINGPLATFORM:
			object = new MovingPlatform;
			break;
		case Entity::ObjectType::OBJECT_TYPE_LOGO:
			object = new Logo;
			break;
		case Entity::ObjectType::OBJECT_TYPE_SELECT:
			object = new SelectText;
			break;
		case Entity::ObjectType::OBJECT_TYPE_CURTAIN:
			object = new Curtain;
			break;
	}

	if (object) {
		object->SetObjectID(static_cast<int>(objectID));
		dynamic_cast<Entity*>(object)->ParseData(tokens.at(1), GetTexturePath(texID), GetTextureColorKey(texID), extraData);
		object->SetPosition(position);
		objects.push_back(object);
		
		/*if (grid) {
			grid->InitObjects(object);
		}*/
	}
}

void Scene::ParseTilesData(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 6) {
		return;
	}

	Tiles* tile = new Tiles;
	tile->SetObjectID(std::stoi(tokens.at(0)));
	tile->SetSpritesArrID(std::stoi(tokens.at(5)));

	float posX, posY;

	RECTF hitbox;
	hitbox.left = posX = std::stof(tokens.at(1));
	hitbox.top = posY = std::stof(tokens.at(2));
	hitbox.right = std::stof(tokens.at(3));
	hitbox.bottom = std::stof(tokens.at(4));

	D3DXVECTOR3 position(posX, posY, 0);

	tile->SetPosition(position);
	tile->AddHitBox(hitbox);

	if (tile) {
		objects.push_back(tile);

		/*if (grid) {
			grid->InitObjects(tile);
		}*/
	}
}

void Scene::ParseTileSprites(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 9) {
		return;
	}

	for (GameObject* object : objects) {
		//check if an object is a tile
		if (dynamic_cast<Tiles*>(object)) {
			Tiles* tile = static_cast<Tiles*>(object);

			int objectID = std::stoi(tokens.at(0));
			int spritesArrID = std::stoi(tokens.at(1));

			RECT bound;
			D3DXVECTOR3 pos;

			if (tile->GetObjectID() == objectID) {
				if (tile->GetSpritesArrID() == spritesArrID) {
					int texID = std::stoi(tokens.at(8));
					tile->LoadTexture(GetTexturePath(texID), GetTextureColorKey(texID));

					bound.left = std::stoi(tokens.at(2));
					bound.top = std::stoi(tokens.at(3));
					bound.right = std::stoi(tokens.at(4)) + 1;
					bound.bottom = std::stoi(tokens.at(5)) + 1;

					float posX = std::stof(tokens.at(6));
					float posY = std::stof(tokens.at(7));
					pos = D3DXVECTOR3(posX, posY, 0);

					tile->AddImage(bound, pos);

					return;
				}
			}
		}
	}
}

void Scene::ParseBackground(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 7) {
		return;
	}

	RECT bound;
	bound.left = std::stoi(tokens.at(0));
	bound.top = std::stoi(tokens.at(1));
	bound.right = std::stoi(tokens.at(2)) + 1;
	bound.bottom = std::stoi(tokens.at(3)) + 1;

	float posX = std::stof(tokens.at(4));
	float posY = std::stof(tokens.at(5));
	D3DXVECTOR3 pos = D3DXVECTOR3(posX, posY, 0);

	int texID = std::stoi(tokens.at(6));
	bgInstance = Background::GetInstance();
	bgInstance->LoadTexture(GetTexturePath(texID), GetTextureColorKey(texID));
	bgInstance->AddImage(bound, pos);
}

void Scene::LoadGrid(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 2) {
		return;
	}

	unsigned int gridWidth = std::stoi(tokens.at(0));
	unsigned int gridHeight = std::stoi(tokens.at(1));

	grid = new Grid(gridWidth, gridHeight);
}

void Scene::LoadGridEntities(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 7) {
		return;
	}

	std::vector<std::string> extraData;

	if (tokens.size() > 7) {
		int objID = std::stoi(tokens.at(2));

		if (objID == 100) {
			for (unsigned int i = 7; i < tokens.size(); ++i) {
				extraData.push_back(tokens.at(i));
			}
		}
		else {
			//(variant) | objID | dataPath | texPath

			unsigned int begin = 7;
			//applies to bricks
			//objID | amount | dataPath ..
			if (objID == 103) {
				extraData.push_back(tokens.at(7));
				extraData.push_back(tokens.at(8));
				begin = 9;
			}

			for (unsigned int i = begin; i < tokens.size(); ++i) {
				if (IsInteger(tokens.at(i))) {
					extraData.push_back(GetTexturePath(std::stoi(tokens.at(i))));
				}
				else {
					extraData.push_back(tokens.at(i));
				}
			}
		}
	}

	int cellX = std::stoi(tokens.at(0));
	int cellY = std::stoi(tokens.at(1));

	float posX = std::stof(tokens.at(5));
	float posY = std::stof(tokens.at(6));
	D3DXVECTOR3 position = D3DXVECTOR3(posX, posY, 0);

	int texID = std::stoi(tokens.at(4));

	GameObject* object = nullptr;

	Entity::ObjectType objectID = static_cast<Entity::ObjectType>(std::stoi(tokens.at(2)));
	switch (objectID) {
		case Entity::ObjectType::OBJECT_TYPE_MARIO:
			marioInstance = Mario::GetInstance();
			marioInstance->SetObjectID(static_cast<int>(objectID));
			marioInstance->ParseData(tokens.at(3), GetTexturePath(texID), GetTextureColorKey(texID), extraData);
			marioInstance->SetPosition(position);
			grid->InitObjects(marioInstance, cellX, cellY);
			break;
		case Entity::ObjectType::OBJECT_TYPE_GOOMBA:
			object = new Goomba;
			break;
		case Entity::ObjectType::OBJECT_TYPE_PARAGOOMBA:
			object = new Paragoomba;
			break;
		case Entity::ObjectType::OBJECT_TYPE_TROOPA:
			object = new KoopaTroopa;
			break;
		case Entity::ObjectType::OBJECT_TYPE_PARATROOPA:
			object = new Parakoopa;
			break;
		case Entity::ObjectType::OBJECT_TYPE_PIPLANT:
			object = new PiranaPlant;
			break;
		case Entity::ObjectType::OBJECT_TYPE_VENUSTRAP:
			object = new VenusFire;
			break;
		case Entity::ObjectType::OBJECT_TYPE_BOOMERBRO:
			object = new BoomerBro;
			break;
		case Entity::ObjectType::OBJECT_TYPE_PORTAL:
			object = new Portal;
			break;
		case Entity::ObjectType::OBJECT_TYPE_COIN:
			object = new Coin;
			break;
		case Entity::ObjectType::OBJECT_TYPE_QUESTIONBLOCK:
			object = new QuestionBlock;
			break;
		case Entity::ObjectType::OBJECT_TYPE_SHINYBRICK:
			object = new ShinyBrick;
			break;
		case Entity::ObjectType::OBJECT_TYPE_BONUSITEM:
			object = new BonusItem;
			break;
		case Entity::ObjectType::OBJECT_TYPE_SWITCHBLOCK:
			object = new SwitchBlock;
			break;
		case Entity::ObjectType::OBJECT_TYPE_CACTUS:
			object = new Cactus;
			break;
		case Entity::ObjectType::OBJECT_TYPE_HELP:
			object = new HelpPopUp;
			break;
		case Entity::ObjectType::OBJECT_TYPE_HAMMERBRONODE:
			object = new HammerBro;
			break;
		case Entity::ObjectType::OBJECT_TYPE_MOVINGPLATFORM:
			object = new MovingPlatform;
			break;
		case Entity::ObjectType::OBJECT_TYPE_LOGO:
			object = new Logo;
			break;
		case Entity::ObjectType::OBJECT_TYPE_SELECT:
			object = new SelectText;
			break;
		case Entity::ObjectType::OBJECT_TYPE_CURTAIN:
			object = new Curtain;
			break;
	}

	if (object) {
		object->SetObjectID(static_cast<int>(objectID));
		dynamic_cast<Entity*>(object)->ParseData(tokens.at(3), GetTexturePath(texID), GetTextureColorKey(texID), extraData);
		object->SetPosition(position);
		objects.push_back(object);
		grid->InitObjects(object, cellX, cellY);
	}
}

void Scene::LoadGridTiles(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 8) {
		return;
	}

	int cellX = std::stoi(tokens.at(0));
	int cellY = std::stoi(tokens.at(1));

	Tiles* tile = new Tiles;
	tile->SetObjectID(std::stoi(tokens.at(2)));
	tile->SetSpritesArrID(std::stoi(tokens.at(7)));

	float posX, posY;

	RECTF hitbox;
	hitbox.left = posX = std::stof(tokens.at(3));
	hitbox.top = posY = std::stof(tokens.at(4));
	hitbox.right = std::stof(tokens.at(5));
	hitbox.bottom = std::stof(tokens.at(6));

	D3DXVECTOR3 position(posX, posY, 0);

	tile->SetPosition(position);
	tile->AddHitBox(hitbox);

	if (tile) {
		objects.push_back(tile);
		grid->InitObjects(tile, cellX, cellY);
	}
}

void Scene::Load(const LPDIRECT3DDEVICE9& device, const LPD3DXSPRITE& handler) {
	char debug[100];
	sprintf_s(debug, "[LOAD] Loading scene ID: %d\n", sceneID);
	OutputDebugStringA(debug);
	
	if (!directDevice) {
		directDevice = device;
	}
	
	if (!spriteHandler) {
		spriteHandler = handler;
	}

	std::ifstream readFile;
	readFile.open(filePath, std::ios::in);

	if (!readFile.is_open()) {
		OutputDebugStringA("Failed to read file\n");
		return;
	}

	GameObject::SetDevice(directDevice);
	GameObject::SetSpriteHandler(spriteHandler);

	AnimatedSprite::SetDevice(directDevice);
	AnimatedSprite::SetSpriteHandler(spriteHandler);

	SceneSection section = SceneSection::SCENE_FILE_SECTION_UNKNOWN;

	cameraInstance = Camera::GetInstance();

	char str[MAX_FILE_LINE];
	while (readFile.getline(str, MAX_FILE_LINE)) {
		std::string line(str);

		if (line.empty() || line.front() == '#') {
			continue;
		}

		if (line == "[MAPSIZE]") {
			section = SceneSection::SCENE_FILE_SECTION_MAPSIZE;
			continue;
		}

		if (line == "[LIMITS]") {
			section = SceneSection::SCENE_FILE_SECTION_CAMERALIMITS;
			continue;
		}

		if (line == "[TIME]") {
			section = SceneSection::SCENE_FILE_SECTION_TIME;
			continue;
		}

		if (line == "[BGCOLOR]") {
			section = SceneSection::SCENE_FILE_SECTION_BGCOLOR;
			continue;
		}

		if (line == "[TEXTURES]") {
			section = SceneSection::SCENE_FILE_SECTION_TEXTURES;
			continue;
		}		

		if (line == "[HUD]") {
			section = SceneSection::SCENE_FILE_SECTION_HUD;
			continue;
		}

		if (line == "[GRID]") {
			section = SceneSection::SCENE_FILE_SECTION_GRID;
			continue;
		}

		if (line == "[ENTITY_DATA]") {
			section = SceneSection::SCENE_FILE_SECTION_ENTITYDATA;
			continue;
		}

		if (line == "[TILES_DATA]") {
			section = SceneSection::SCENE_FILE_SECTION_TILESDATA;
			continue;
		}		

		if (line == "[TILE_SPRITES_ARR]") {
			section = SceneSection::SCENE_FILE_SECTION_TILESPRITES;
			continue;
		}

		if (line == "[BACKGROUND]") {
			section = SceneSection::SCENE_FILE_SECTION_BACKGROUND;
			continue;
		}

		switch (section) {
			case SceneSection::SCENE_FILE_SECTION_MAPSIZE:
				ParseMapSize(line);
				break;
			case SceneSection::SCENE_FILE_SECTION_CAMERALIMITS:
				ParseCameraLimits(line);
				break;
			case SceneSection::SCENE_FILE_SECTION_TIME:
				ParseSceneTime(line);
				break;
			case SceneSection::SCENE_FILE_SECTION_BGCOLOR:
				ParseBGColor(line);
				break;
			case SceneSection::SCENE_FILE_SECTION_TEXTURES:
				ParseTextures(line);
				break;
			case SceneSection::SCENE_FILE_SECTION_HUD:
				ParseHUD(line);
				break;
			case SceneSection::SCENE_FILE_SECTION_GRID:
				ParseGrid(line);
				break;
			case SceneSection::SCENE_FILE_SECTION_ENTITYDATA:
				ParseEntityData(line);
				break;
			case SceneSection::SCENE_FILE_SECTION_TILESDATA:
				ParseTilesData(line);
				break;
			case SceneSection::SCENE_FILE_SECTION_TILESPRITES:
				ParseTileSprites(line);
				break;
			case SceneSection::SCENE_FILE_SECTION_BACKGROUND:
				ParseBackground(line);
				break;
		}
	}

	readFile.close();
}

void Scene::Unload() {
	char debug[100];
	sprintf_s(debug, "[UNLOAD] Unloading, scene ID: %d\n", sceneID);
	OutputDebugStringA(debug);

	if (bgInstance) {
		bgInstance->Release();
	}

	if (hudInstance) {
		hudInstance->Release();
	}

	if (marioInstance) {
		marioInstance->Release();
	}

	if (cameraInstance) {
		cameraInstance->Release();
	}

	activeObjects.clear();
	for (GameObject* object : objects) {
		if (object) {
			object->Release();
		}
	}
	objects.clear();

	textureFiles.clear();

	sprintf_s(debug, "[UNLOAD] Unloaded scene: %s\n", filePath.c_str());
	OutputDebugStringW(Util::ToLPCWSTR(debug));
}

void Scene::UpdateCameraPosition(DWORD delta) {	
	int ind = 0;
	if (marioInstance->WentIntoPipe()) {
		ind = 1;
	}
	else {
		ind = 0;
	}

	D3DXVECTOR3 camPosition = cameraInstance->GetPosition();

	switch (static_cast<SceneType>(sceneID)) {
		case SceneType::SCENE_SANDBOX:
		case SceneType::SCENE_STAGEONE:
			if (!marioInstance->TriggeredStageEnd() && !marioInstance->IsInPipe()) {
				//set mario's position inside the world
				if (marioInstance->GetPosition().x < cameraInstance->GetPosition().x) {
					marioInstance->SetPosition(D3DXVECTOR3(cameraInstance->GetPosition().x, marioInstance->GetPosition().y, 0));
				}
				else if ((marioInstance->GetPosition().x + marioInstance->GetBoxWidth()) > sceneWidth) {
					marioInstance->SetPosition(D3DXVECTOR3(static_cast<float>(sceneWidth) - marioInstance->GetBoxWidth(), marioInstance->GetPosition().y, 0));
				}
			}

			camPosition = marioInstance->GetPosition();
			
			camPosition.x -= Game::GetInstance()->GetScreenWidth() / 2.0f;
			if (camPosition.x < cameraInstance->GetLimit(ind).left) {
				camPosition.x = cameraInstance->GetLimit(ind).left;
			}
			else if (camPosition.x + Game::GetInstance()->GetScreenWidth() > cameraInstance->GetLimit(ind).right) {
				camPosition.x = cameraInstance->GetLimit(ind).right - Game::GetInstance()->GetScreenWidth();
			}

			camPosition.y -= Game::GetInstance()->GetScreenHeight() / 2.0f;

			if (marioInstance->IsFlying() || marioInstance->GetPosition().y < sceneHeight / 3.4f) {
				if (camPosition.y < cameraInstance->GetLimit(ind).top) {
					camPosition.y = cameraInstance->GetLimit(ind).top;
				}
				else if (camPosition.y + Game::GetInstance()->GetScreenHeight() > cameraInstance->GetLimit(ind).bottom) {
					camPosition.y = cameraInstance->GetLimit(ind).bottom - Game::GetInstance()->GetScreenHeight();
				}
			}
			else {
				camPosition.y = cameraInstance->GetLimit(ind).bottom - Game::GetInstance()->GetScreenHeight();
			}

			cameraInstance->SetPosition(camPosition);
			break;
		case SceneType::SCENE_STAGEFOUR:		
			if (marioInstance->GetCurrentHitPoints() > 0 && camPosition.x + Game::GetInstance()->GetScreenWidth() < cameraInstance->GetLimit(0).right) {
				cameraInstance->Update(delta);

				if (!marioInstance->TriggeredStageEnd()) {
					//set mario's position inside the world
					if (marioInstance->GetPosition().x < cameraInstance->GetPosition().x) {
						marioInstance->SetPosition(D3DXVECTOR3(cameraInstance->GetPosition().x, marioInstance->GetPosition().y, 0));
					}
					else if ((marioInstance->GetPosition().x + marioInstance->GetBoxWidth()) > cameraInstance->GetPosition().x + Game::GetInstance()->GetScreenWidth()) {
						marioInstance->SetPosition(D3DXVECTOR3(marioInstance->GetPosition().x - marioInstance->GetBoxWidth(), marioInstance->GetPosition().y, 0));
					}
				}
			}
			else {
				if (ind == 1) {
					if (!marioInstance->TriggeredStageEnd()) {
						//set mario's position inside the world
						if (marioInstance->GetPosition().x < cameraInstance->GetPosition().x) {
							marioInstance->SetPosition(D3DXVECTOR3(cameraInstance->GetPosition().x, marioInstance->GetPosition().y, 0));
						}
						else if ((marioInstance->GetPosition().x + marioInstance->GetBoxWidth()) > sceneWidth) {
							marioInstance->SetPosition(D3DXVECTOR3(static_cast<float>(sceneWidth) - marioInstance->GetBoxWidth(), marioInstance->GetPosition().y, 0));
						}
					}

					camPosition = marioInstance->GetPosition();

					camPosition.x -= Game::GetInstance()->GetScreenWidth() / 2.0f;
					if (camPosition.x < cameraInstance->GetLimit(ind).left) {
						camPosition.x = cameraInstance->GetLimit(ind).left;
					}
					else if (camPosition.x + Game::GetInstance()->GetScreenWidth() > cameraInstance->GetLimit(ind).right) {
						camPosition.x = cameraInstance->GetLimit(ind).right - Game::GetInstance()->GetScreenWidth();
					}
				}

				camPosition.y -= Game::GetInstance()->GetScreenHeight() / 2.0f;
				camPosition.y = cameraInstance->GetLimit(ind).bottom - Game::GetInstance()->GetScreenHeight();

				cameraInstance->SetPosition(camPosition);
			}

			/*camPosition = marioInstance->GetPosition();
			camPosition.x -= Game::GetInstance()->GetScreenWidth() / 2.0f;
			camPosition.y -= Game::GetInstance()->GetScreenHeight() / 2.0f;
			camPosition.y = cameraInstance->GetLimit(ind).bottom - Game::GetInstance()->GetScreenHeight();
			cameraInstance->SetPosition(camPosition);*/
			break;
	}
}

void Scene::UpdateHUDPosition(DWORD delta) {
	float offset = 0.0f;
	switch (static_cast<SceneType>(sceneID)) {
		case SceneType::SCENE_MAP:
			offset = 185.0f;
			break;
		case SceneType::SCENE_STAGEONE:
		case SceneType::SCENE_STAGEFOUR:
			offset = 195.0f;
			break;
	}

	D3DXVECTOR3 hudPosition;
	hudPosition.x = cameraInstance->GetPosition().x + 6.0f;
	hudPosition.y = cameraInstance->GetPosition().y + offset;

	hudInstance->SetPosition(hudPosition);
}

void Scene::Update(DWORD delta) {
	if (!marioInstance) {
		char debug[100];
		sprintf_s(debug, "[UPDATE] No mario, scene ID: %d\n", sceneID);
		OutputDebugStringA(debug);
		return;
	}	

	std::vector<GameObject*> collidableObjects;
	for (GameObject* object : objects) {
		collidableObjects.push_back(object);
	}

	/*if (grid) {
		grid->Update();
		activeObjects = GetActiveObjects();
	}*/

	switch (static_cast<SceneType>(sceneID)) {
		case SceneType::SCENE_INTRO:
			{
				std::string mode = "1Player";

				for (unsigned int i = 0; i < objects.size(); ++i) {
					objects.at(i)->Update(delta, &collidableObjects);

					if (dynamic_cast<SelectText*>(objects.at(i))) {
						SelectText* select = static_cast<SelectText*>(objects.at(i));

						if (Device::IsKeyDown(DIK_W)) {
							select->Select1PMode();
						}
						else if (Device::IsKeyDown(DIK_S)) {
							select->Select2PMode();
						}

						mode = select->GetMode();
					}
				}

				if (Device::IsKeyDown(DIK_I)) {
					SceneManager::GetInstance()->ChangeScene(static_cast<unsigned int>(mode == "1Player" ? SceneType::SCENE_MAP : SceneType::SCENE_OVER));
				}
			}			
			break;
		case SceneType::SCENE_MAP:
			marioInstance->Update(delta, &collidableObjects);

			if (hudInstance) {
				//should've passed mario in
				hudInstance->Update(
					delta,
					marioInstance->GetLivesLeft(),
					marioInstance->GetCoinsCollected(),
					marioInstance->GetBonusItems(),
					marioInstance->GetAcceleration(),
					marioInstance->GetAccelThreshold(),
					marioInstance->GetCurrentScore(),
					sceneTime,
					marioInstance->IsFlying(),
					marioInstance->IsRunningKeyPressed(),
					marioInstance->TriggeredStageEnd()
				);

				UpdateHUDPosition(delta);
			}

			for (unsigned int i = 0; i < objects.size(); ++i) {
				objects.at(i)->Update(delta, &collidableObjects);
			}

			if (marioInstance->IsInStageNode()) {
				SceneManager::GetInstance()->ChangeScene(static_cast<unsigned int>(marioInstance->GetNextSceneID()));
			}
			break;
		case SceneType::SCENE_STAGEONE:
		case SceneType::SCENE_STAGEFOUR:
			if (marioInstance->GetCurrentHitPoints() > 0 && !marioInstance->TriggeredStageEnd()) {
				//scene timer
				if (GetTickCount64() % 1000 == 0 && sceneTime > 0) {
					sceneTime -= 1;
				}
			}

			marioInstance->Update(delta, &collidableObjects);

			hudInstance->Update(
				delta,
				marioInstance->GetLivesLeft(),
				marioInstance->GetCoinsCollected(),
				marioInstance->GetBonusItems(),
				marioInstance->GetAcceleration(),
				marioInstance->GetAccelThreshold(),
				marioInstance->GetCurrentScore(),
				sceneTime,
				marioInstance->IsFlying(),
				marioInstance->IsRunningKeyPressed(),
				marioInstance->TriggeredStageEnd()
			);

			if (marioInstance->GetCurrentHitPoints() > 0) {
				for (unsigned int i = 0; i < objects.size(); ++i) {
					objects.at(i)->Update(delta, &objects);

					//add score if mario hits the entity
					if (dynamic_cast<Entity*>(objects.at(i))) {
						Entity* entity = static_cast<Entity*>(objects.at(i));
						if (entity->TookDamage()) {
							marioInstance->AddScore(entity->GetScore());
						}
					}

					if (dynamic_cast<QuestionBlock*>(objects.at(i))) {
						QuestionBlock* questionBlock = static_cast<QuestionBlock*>(objects.at(i));
						if (questionBlock->TookDamage()) {
							Entity* item = questionBlock->SpawnItem(marioInstance->GetCurrentHitPoints());
							AddObjectToScene(item);
						}
					}

					if (dynamic_cast<ShinyBrick*>(objects.at(i))) {
						ShinyBrick* shinyBrick = static_cast<ShinyBrick*>(objects.at(i));
						if (shinyBrick->TookDamage()) {
							Entity* item = shinyBrick->SpawnItem();
							AddObjectToScene(item);
						}
					}

					if (dynamic_cast<Paragoomba*>(objects.at(i))) {
						Paragoomba* paraGoomba = static_cast<Paragoomba*>(objects.at(i));
						if (paraGoomba->IsTired() && paraGoomba->GetCurrentHitPoints() == 2) {
							//mario is on the right side of goomba
							if ((paraGoomba->GetPosition().x - marioInstance->GetPosition().x) < 0) {
								paraGoomba->SetNormal(D3DXVECTOR3(-1, 0, 0));
							}
							else {
								paraGoomba->SetNormal(D3DXVECTOR3(1, 0, 0));
							}
						}
					}

					if (dynamic_cast<KoopaTroopa*>(objects.at(i))) {
						KoopaTroopa* koopaTroopa = static_cast<KoopaTroopa*>(objects.at(i));
						if (koopaTroopa->GetCurrentHitPoints() == 2) {
							//mario is on the right side of koopa
							if ((koopaTroopa->GetPosition().x - marioInstance->GetPosition().x) < 0) {
								koopaTroopa->SetNormal(D3DXVECTOR3(-1, 0, 0));
							}
							else {
								koopaTroopa->SetNormal(D3DXVECTOR3(1, 0, 0));
							}
						}
					}

					if (dynamic_cast<PiranaPlant*>(objects.at(i))) {
						PiranaPlant* piranaPlant = static_cast<PiranaPlant*>(objects.at(i));

						/*char debug[100];
						sprintf_s(debug, "Pirana posx: %f\tMario posx: %f\n", piranaPlant->GetPosition().x - 72, marioInstance->GetPosition().x + marioInstance->GetBoxWidth());
						OutputDebugStringA(debug);*/

						if (((marioInstance->GetPosition().x + marioInstance->GetBoxWidth()) >= piranaPlant->GetPosition().x - 8) &&
							(marioInstance->GetPosition().x <= piranaPlant->GetPosition().x + piranaPlant->GetBoxWidth() * 2))
						{
							piranaPlant->PlayerIsInRange(true);
						}
						else {
							piranaPlant->PlayerIsInRange(false);
						}
					}

					if (dynamic_cast<VenusFire*>(objects.at(i))) {
						VenusFire* venusFire = static_cast<VenusFire*>(objects.at(i));
						//mario is on the right side of venus
						if ((venusFire->GetPosition().x - marioInstance->GetPosition().x) < 0) {
							venusFire->SetNormal(D3DXVECTOR3(-1, venusFire->GetNormal().y, 0));
						}
						else {
							venusFire->SetNormal(D3DXVECTOR3(1, venusFire->GetNormal().y, 0));
						}

						//mario is under venus
						if ((venusFire->GetPosition().y - marioInstance->GetPosition().y) < 0) {
							venusFire->SetNormal(D3DXVECTOR3(venusFire->GetNormal().x, -1, 0));
						}
						else {
							venusFire->SetNormal(D3DXVECTOR3(venusFire->GetNormal().x, 1, 0));
						}
					}

					if (dynamic_cast<Entity*>(objects.at(i)) && dynamic_cast<Entity*>(objects.at(i))->GetCurrentHitPoints() == -1) {
						if (dynamic_cast<Coin*>(objects.at(i))) {
							marioInstance->AddCoin();
						}

						//erase-remove idiom
						objects.at(i)->Release();
						objects.erase(std::remove(objects.begin(), objects.end(), objects.at(i)), objects.end());
					}
				}
			}

			UpdateCameraPosition(delta);
			UpdateHUDPosition(delta);

			//change scene to overworld map
			if (marioInstance->TriggeredStageEnd() || marioInstance->GetCurrentHitPoints() == 0 || GetSceneTime() == 0) {
				if (!IsTranstionStarting()) {
					if (marioInstance->TriggeredStageEnd()) {
						marioInstance->GetSceneRemainingTime(sceneTime);
					}

					StartChangeSceneToMapTimer();
				}

				if (toMapStart != 0 && GetTickCount64() - toMapStart > toMapTime) {
					toMapStart = 0;
					SceneManager::GetInstance()->ChangeScene(static_cast<unsigned int>(SceneType::SCENE_MAP));
				}
			}
			break;
	}

	switch (static_cast<SceneType>(sceneID)) {
		case SceneType::SCENE_SANDBOX:
			marioInstance->Update(delta, &collidableObjects);

			if (hudInstance) {
				hudInstance->Update(
					delta,
					marioInstance->GetLivesLeft(),
					marioInstance->GetCoinsCollected(),
					marioInstance->GetBonusItems(),
					marioInstance->GetAcceleration(),
					marioInstance->GetAccelThreshold(),
					marioInstance->GetCurrentScore(),
					sceneTime,
					marioInstance->IsFlying(),
					marioInstance->IsRunningKeyPressed(),
					marioInstance->TriggeredStageEnd()
				);

				UpdateHUDPosition(delta);
			}

			UpdateCameraPosition(delta);
			UpdateHUDPosition(delta);
			break;
	}
}

void Scene::Render() {
	//Order of rendering:
	//Background
	//Piranas and Venus
	//Tiles
	//Mushrooms
	//NPCs
	//Mario
	//HUD

	if (bgInstance) {
		bgInstance->Render();
	}
	
	if (marioInstance) {
		if (marioInstance->IsInPipe()) {
			marioInstance->Render();
		}
	}

	for (GameObject* object : objects) {
		object->Render();
	}

	if (marioInstance) {
		if (!marioInstance->IsInPipe()) {
			marioInstance->Render();
		}
	}
	
	if (hudInstance) {
		hudInstance->Render();
	}
}

void Scene::HandleStates(BYTE* states) {
	if (!marioInstance->TriggeredStageEnd()) {
		marioInstance->HandleStates(states);
	}
}

void Scene::OnKeyDown(int keyCode) {
	if (!marioInstance->TriggeredStageEnd()) {
		marioInstance->OnKeyDown(keyCode);
	}
}

void Scene::OnKeyUp(int keyCode) {
	marioInstance->OnKeyUp(keyCode);
}
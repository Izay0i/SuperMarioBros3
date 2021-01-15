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

void Scene::ParseEntityData(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 3) {
		return;
	}

	std::vector<std::string> extraData;

	if (tokens.size() > 3) {
		int objID = std::stoi(tokens.at(0));

		if (objID == 100) {
			for (unsigned int i = 3; i < tokens.size(); ++i) {
				extraData.push_back(tokens.at(i));
			}
		}
		else {
			//(variant) | objID | dataPath | texPath

			unsigned int begin = 3;
			//applies to bricks
			//objID | amount | dataPath ..
			if (objID == 103) {
				extraData.push_back(tokens.at(3));
				extraData.push_back(tokens.at(4));
				begin = 5;
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

	int texID = std::stoi(tokens.at(2));

	GameObject* object = nullptr;

	Entity::ObjectType objectID = static_cast<Entity::ObjectType>(std::stoi(tokens.at(0)));
	switch (objectID) {
		case Entity::ObjectType::OBJECT_TYPE_MARIO:
			marioInstance = Mario::GetInstance();			
			marioInstance->SetObjectID(static_cast<int>(objectID));
			marioInstance->ParseData(tokens.at(1), GetTexturePath(texID), GetTextureColorKey(texID), extraData);
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

		//testing
		case Entity::ObjectType::OBJECT_TYPE_MUSHROOM:
			object = new SuperMushroom;
			break;
		case Entity::ObjectType::OBJECT_TYPE_1UPSHROOM:
			object = new GMushroom;
			break;
		case Entity::ObjectType::OBJECT_TYPE_LEAF:
			object = new SuperLeaf;
			break;
	}

	if (object) {
		object->SetObjectID(static_cast<int>(objectID));
		dynamic_cast<Entity*>(object)->ParseData(tokens.at(1), GetTexturePath(texID), GetTextureColorKey(texID), extraData);
		objects.push_back(object);
	}
}

void Scene::ParseWorldCoords(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 3) {
		return;
	}

	float posX = std::stof(tokens.at(1));
	float posY = std::stof(tokens.at(2));
	D3DXVECTOR3 position(posX, posY, 0);

	Entity::ObjectType objectID = static_cast<Entity::ObjectType>(std::stoi(tokens.at(0)));
	switch (objectID) {
		case Entity::ObjectType::OBJECT_TYPE_MARIO:
			marioInstance->SetPosition(position);
			break;
		case Entity::ObjectType::OBJECT_TYPE_GOOMBA:
			for (GameObject* object : objects) {
				if (dynamic_cast<Goomba*>(object) && object->GetPosition() == D3DXVECTOR3(0, 0, 0)) {
					int objectID = std::stoi(tokens.at(0));
					if (object->GetObjectID() == objectID) {
						object->SetPosition(position);
					}
					return;
				}
			}
			break;
		case Entity::ObjectType::OBJECT_TYPE_PARAGOOMBA:
			for (GameObject* object : objects) {
				if (dynamic_cast<Paragoomba*>(object)) {
					int objectID = std::stoi(tokens.at(0));
					if (object->GetObjectID() == objectID) {
						object->SetPosition(position);
					}
					return;
				}
			}
			break;
		case Entity::ObjectType::OBJECT_TYPE_TROOPA:
			for (GameObject* object : objects) {
				if (dynamic_cast<KoopaTroopa*>(object) && object->GetPosition() == D3DXVECTOR3(0, 0, 0)) {
					int objectID = std::stoi(tokens.at(0));
					if (object->GetObjectID() == objectID) {
						object->SetPosition(position);
					}
					return;
				}
			}
			break;
		case Entity::ObjectType::OBJECT_TYPE_PARATROOPA:
			for (GameObject* object : objects) {
				if (dynamic_cast<Parakoopa*>(object) && object->GetPosition() == D3DXVECTOR3(0, 0, 0)) {
					int objectID = std::stoi(tokens.at(0));
					if (object->GetObjectID() == objectID) {
						object->SetPosition(position);
					}
					return;
				}
			}
			break;
		case Entity::ObjectType::OBJECT_TYPE_PIPLANT:
			for (GameObject* object : objects) {
				if (dynamic_cast<PiranaPlant*>(object) && object->GetPosition() == D3DXVECTOR3(0, 0, 0)) {
					int objectID = std::stoi(tokens.at(0));
					if (object->GetObjectID() == objectID) {
						object->SetPosition(position);
					}
					return;
				}
			}
			break;
		case Entity::ObjectType::OBJECT_TYPE_VENUSTRAP:
			for (GameObject* object : objects) {
				if (dynamic_cast<VenusFire*>(object) && object->GetPosition() == D3DXVECTOR3(0, 0, 0)) {
					int objectID = std::stoi(tokens.at(0));
					if (object->GetObjectID() == objectID) {
						object->SetPosition(position);
					}
					return;
				}
			}
			break;
		case Entity::ObjectType::OBJECT_TYPE_PORTAL:
			for (GameObject* object : objects) {
				if (dynamic_cast<Portal*>(object) && object->GetPosition() == D3DXVECTOR3(0, 0, 0)) {
					int objectID = std::stoi(tokens.at(0));
					if (object->GetObjectID() == objectID) {
						object->SetPosition(position);
					}
					return; //dunno why i put the return statement here
				}
			}
			break;
		case Entity::ObjectType::OBJECT_TYPE_COIN:
			for (GameObject* object : objects) {
				//dumbass way to check if position is not set
				//but its almost 2am and im too tired to think of another way
				if (dynamic_cast<Coin*>(object) && object->GetPosition() == D3DXVECTOR3(0, 0, 0)) {
					int objectID = std::stoi(tokens.at(0));
					if (object->GetObjectID() == objectID) {					
						object->SetPosition(position);
					}
					return;
				}
			}
			break;
		case Entity::ObjectType::OBJECT_TYPE_QUESTIONBLOCK:
			for (GameObject* object : objects) {
				if (dynamic_cast<QuestionBlock*>(object) && object->GetPosition() == D3DXVECTOR3(0, 0, 0)) {
					int objectID = std::stoi(tokens.at(0));
					if (object->GetObjectID() == objectID) {
						object->SetPosition(position);
					}
					return;
				}
			}
			break;
		case Entity::ObjectType::OBJECT_TYPE_SHINYBRICK:
			for (GameObject* object : objects) {
				if (dynamic_cast<ShinyBrick*>(object) && object->GetPosition() == D3DXVECTOR3(0, 0, 0)) {
					int objectID = std::stoi(tokens.at(0));
					if (object->GetObjectID() == objectID) {
						object->SetPosition(position);
					}
					return;
				}
			}
			break;
		case Entity::ObjectType::OBJECT_TYPE_BONUSITEM:
			for (GameObject* object : objects) {
				if (dynamic_cast<BonusItem*>(object)) {
					int objectID = std::stoi(tokens.at(0));
					if (object->GetObjectID() == objectID) {
						object->SetPosition(position);
					}
					return;
				}
			}
			break;
		case Entity::ObjectType::OBJECT_TYPE_SWITCHBLOCK:
			for (GameObject* object : objects) {
				if (dynamic_cast<SwitchBlock*>(object)) {
					int objectID = std::stoi(tokens.at(0));
					if (object->GetObjectID() == objectID) {
						object->SetPosition(position);
					}
					return;
				}
			}
			break;

		case Entity::ObjectType::OBJECT_TYPE_MUSHROOM:
			for (GameObject* object : objects) {
				if (dynamic_cast<SuperMushroom*>(object) && object->GetPosition() == D3DXVECTOR3(0, 0, 0)) {
					int objectID = std::stoi(tokens.at(0));
					if (object->GetObjectID() == objectID) {
						object->SetPosition(position);
					}
					return;
				}
			}
			break;
		case Entity::ObjectType::OBJECT_TYPE_1UPSHROOM:
			for (GameObject* object : objects) {
				if (dynamic_cast<GMushroom*>(object) && object->GetPosition() == D3DXVECTOR3(0, 0, 0)) {
					int objectID = std::stoi(tokens.at(0));
					if (object->GetObjectID() == objectID) {
						object->SetPosition(position);
					}
					return;
				}
			}
			break;
		case Entity::ObjectType::OBJECT_TYPE_LEAF:
			for (GameObject* object : objects) {
				if (dynamic_cast<SuperLeaf*>(object) && object->GetPosition() == D3DXVECTOR3(0, 0, 0)) {
					int objectID = std::stoi(tokens.at(0));
					if (object->GetObjectID() == objectID) {
						object->SetPosition(position);
					}
					return;
				}
			}
			break;
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

	char str[MAX_FILE_LINE];
	while (readFile.getline(str, MAX_FILE_LINE)) {
		std::string line(str);

		if (line.empty() || line.front() == '#') {
			continue;
		}

		if (line == "[BGCOLOR]") {
			section = SceneSection::SCENE_FILE_SECTION_BGCOLOR;
			continue;
		}

		if (line == "[MAPSIZE]") {
			section = SceneSection::SCENE_FILE_SECTION_MAPSIZE;
			continue;
		}

		if (line == "[TIME]") {
			section = SceneSection::SCENE_FILE_SECTION_TIME;
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

		if (line == "[ENTITY_DATA]") {
			section = SceneSection::SCENE_FILE_SECTION_ENTITYDATA;
			continue;
		}

		if (line == "[WORLD_COORDS]") {
			section = SceneSection::SCENE_FILE_SECTION_WORLDCOORDS;
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
			case SceneSection::SCENE_FILE_SECTION_ENTITYDATA:
				ParseEntityData(line);
				break;			
			case SceneSection::SCENE_FILE_SECTION_WORLDCOORDS:
				ParseWorldCoords(line);
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

	cameraInstance = Camera::GetInstance();

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

void Scene::UpdateCameraPosition() {
	if (marioInstance->GetPosition().x < 16.0f) {
		marioInstance->SetPosition(D3DXVECTOR3(16.0f, marioInstance->GetPosition().y, 0));
	}
	else if ((marioInstance->GetPosition().x + marioInstance->GetBoxWidth()) > sceneWidth) {
		marioInstance->SetPosition(D3DXVECTOR3(static_cast<float>(sceneWidth) - marioInstance->GetBoxWidth(), marioInstance->GetPosition().y, 0));
	}

	D3DXVECTOR3 camPosition = marioInstance->GetPosition();
	camPosition.x -= Game::GetInstance()->GetScreenWidth() / 2.0f;
	//half a gap in the viewport just like in the original
	if (camPosition.x < 8.0f) {
		camPosition.x = 8.0f;
	}
	else if ((camPosition.x + Game::GetInstance()->GetScreenWidth()) > sceneWidth) {
		camPosition.x = static_cast<float>(sceneWidth) - Game::GetInstance()->GetScreenWidth();
	}
	
	camPosition.y -= Game::GetInstance()->GetScreenHeight() / 4.0f;
	//testing
	/*if (camPosition.y > sceneHeight - Game::GetInstance()->GetScreenHeight()) {
		camPosition.y = static_cast<float>(sceneHeight) - Game::GetInstance()->GetScreenHeight();
	}
	else if (camPosition.y < 0.0f) {
		camPosition.y = 0.0f;
	}*/

	if (marioInstance->IsFlying() || (marioInstance->GetPosition().y + marioInstance->GetBoxHeight() < (sceneHeight / 3.5f))) {
		if (camPosition.y > sceneHeight - Game::GetInstance()->GetScreenHeight()) {
			camPosition.y = static_cast<float>(sceneHeight) - Game::GetInstance()->GetScreenHeight();
		}
		else if (camPosition.y < 0.0f) {
			camPosition.y = 0.0f;
		}
	}
	else {
		camPosition.y = 239.0f;
	}

	/*char debugStr[100];
	sprintf_s(debugStr, "Cam y: %f\n", camPosition.y);
	OutputDebugStringA(debugStr);*/

	cameraInstance->SetPosition(camPosition);
}

void Scene::UpdateHUDPosition() {
	D3DXVECTOR3 hudPosition = cameraInstance->GetPosition();
	//hudPosition.x = marioInstance->GetPosition().x;
	//hudPosition.y = Camera::GetInstance()->GetPosition().y + 145.0f;
	hudPosition.y = 435;

	hudInstance->SetPosition(hudPosition);
}

void Scene::Update(DWORD delta) {
	if (!marioInstance) {
		/*char debug[100];
		sprintf_s(debug, "[UPDATE] No mario, scene ID: %d\n", sceneID);
		OutputDebugStringA(debug);*/
		return;
	}

	std::vector<GameObject*> collidableObjects;
	for (GameObject* object : objects) {
		collidableObjects.push_back(object);
	}

	switch (static_cast<SceneType>(sceneID)) {
		case SceneType::SCENE_INTRO:

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
			}

			if (marioInstance->GetCurrentHitPoints() > 0) {
				for (unsigned int i = 0; i < objects.size(); ++i) {
					objects.at(i)->Update(delta, &collidableObjects);

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

					if (dynamic_cast<Entity*>(objects.at(i))) {
						if (dynamic_cast<Entity*>(objects.at(i))->GetCurrentHitPoints() == -1) {
							//erase-remove idiom
							objects.at(i)->Release();
							objects.erase(std::remove(objects.begin(), objects.end(), objects.at(i)), objects.end());
						}
					}
				}
			}

			if (!marioInstance->TriggeredStageEnd()) {
				UpdateCameraPosition();
			}
			UpdateHUDPosition();

			//change scene to overworld map
			if (marioInstance->TriggeredStageEnd() || marioInstance->GetCurrentHitPoints() == 0) {
				//SceneManager::GetInstance()->ChangeScene(static_cast<unsigned int>(SceneType::SCENE_MAP));

				if (!IsTranstionStarting()) {
					StartChangeSceneToMapTimer();
				}

				if (toMapStart != 0 && GetTickCount64() - toMapStart > toMapTime) {
					SceneManager::GetInstance()->ChangeScene(static_cast<unsigned int>(SceneType::SCENE_MAP));
				}
			}
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
	
	for (GameObject* object : objects) {
		if (!dynamic_cast<Entity*>(object)) {
			object->Render();
		}
	}

	for (GameObject* object : objects) {
		if (dynamic_cast<Entity*>(object)) {
			object->Render();
		}
	}
	
	for (GameObject* object : objects) {
		if (dynamic_cast<PiranaPlant*>(object)) {
			object->Render();
		}
	}

	if (marioInstance) {
		marioInstance->Render();
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
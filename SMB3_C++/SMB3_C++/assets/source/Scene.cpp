#include "../headers/Scene.h"

LPDIRECT3DDEVICE9 Scene::directDevice = nullptr;
LPD3DXSPRITE Scene::spriteHandler = nullptr;

Scene::Scene(int id, std::string path) {
	sceneID = id;
	filePath = path;

	bgInstance = new Background;
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

int Scene::GetSceneWidth() const {
	return sceneWidth;
}

int Scene::GetSceneHeight() const {
	return sceneHeight;
}

void Scene::AddObjectToScene(GameObject* object) {
	objects.push_back(object);
}

void Scene::ParseMapSize(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 2) {
		return;
	}

	sceneWidth = std::stoi(tokens.at(0));
	sceneHeight = std::stoi(tokens.at(1));
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

void Scene::ParseEntityData(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 3) {
		return;
	}

	int texID = std::stoi(tokens.at(2));

	GameObject* object = nullptr;

	ObjectType objectID = static_cast<ObjectType>(std::stoi(tokens.at(0)));
	switch (objectID) {
		case ObjectType::OBJECT_TYPE_MARIO:
			marioInstance = Mario::GetInstance();			
			marioInstance->SetObjectID(static_cast<int>(objectID));
			marioInstance->ParseData(tokens.at(1), tokens.at(3), GetTexturePath(texID), GetTextureColorKey(texID));
			break;
		case ObjectType::OBJECT_TYPE_GOOMBA:
			object = new Goomba;
			break;
		case ObjectType::OBJECT_TYPE_REDPARAGOOMBA:
			
			break;
		case ObjectType::OBJECT_TYPE_TROOPA:
			object = new KoopaTroopa;
			break;
		case ObjectType::OBJECT_TYPE_PARATROOPA:

			break;
		case ObjectType::OBJECT_TYPE_PIPLANT:

			break;
		case ObjectType::OBJECT_TYPE_VENUSTRAP:

			break;
		case ObjectType::OBJECT_TYPE_COIN:
			object = new Coin;
			break;
		case ObjectType::OBJECT_TYPE_QUESTIONBLOCK:
			object = new QuestionBlock;
			break;
		case ObjectType::OBJECT_TYPE_SHINYBRICK:
			object = new ShinyBrick;
			break;
		case ObjectType::OBJECT_TYPE_BONUSITEM:
			object = new BonusItem;			
			break;	
	}

	if (object) {
		object->SetObjectID(static_cast<int>(objectID));
		dynamic_cast<Entity*>(object)->ParseData(tokens.at(1), GetTexturePath(texID), GetTextureColorKey(texID));
		objects.push_back(object);
	}
}

void Scene::ParseWorldCoords(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 3) {
		return;
	}

	int posX = std::stoi(tokens.at(1));
	int posY = std::stoi(tokens.at(2));
	D3DXVECTOR3 position(posX, posY, 0);

	ObjectType objectID = static_cast<ObjectType>(std::stoi(tokens.at(0)));
	switch (objectID) {
		case ObjectType::OBJECT_TYPE_MARIO:
			marioInstance->SetPosition(position);
			break;
		case ObjectType::OBJECT_TYPE_GOOMBA:
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
		case ObjectType::OBJECT_TYPE_REDPARAGOOMBA:

			break;
		case ObjectType::OBJECT_TYPE_TROOPA:
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
		case ObjectType::OBJECT_TYPE_PARATROOPA:

			break;
		case ObjectType::OBJECT_TYPE_PIPLANT:

			break;
		case ObjectType::OBJECT_TYPE_VENUSTRAP:

			break;
		case ObjectType::OBJECT_TYPE_COIN:
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
		case ObjectType::OBJECT_TYPE_QUESTIONBLOCK:
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
		case ObjectType::OBJECT_TYPE_SHINYBRICK:
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
		case ObjectType::OBJECT_TYPE_BONUSITEM:
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
	}
}

void Scene::ParseTilesData(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 6) {
		return;
	}

	GameObject* object = new Tiles;
	object->SetObjectID(std::stoi(tokens.at(0)));
	dynamic_cast<Tiles*>(object)->SetSpritesArrID(std::stoi(tokens.at(5)));

	int posX, posY;

	RECTF hitbox;
	hitbox.left = posX = std::stoi(tokens.at(1));
	hitbox.top = posY = std::stoi(tokens.at(2));
	hitbox.right = std::stoi(tokens.at(3));
	hitbox.bottom = std::stoi(tokens.at(4));

	D3DXVECTOR3 position(posX, posY, 0);

	object->SetPosition(position);
	dynamic_cast<Tiles*>(object)->AddHitBox(hitbox);

	if (object) {
		objects.push_back(object);
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

					int posX = std::stoi(tokens.at(6));
					int posY = std::stoi(tokens.at(7));
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

	int posX = std::stoi(tokens.at(4));
	int posY = std::stoi(tokens.at(5));
	D3DXVECTOR3 pos = D3DXVECTOR3(posX, posY, 0);

	int texID = std::stoi(tokens.at(6));
	bgInstance->LoadTexture(GetTexturePath(texID), GetTextureColorKey(texID));
	bgInstance->AddImage(bound, pos);
}

void Scene::Load(const LPDIRECT3DDEVICE9& device, const LPD3DXSPRITE& handler) {
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

		if (line == "[TEXTURES]") {
			section = SceneSection::SCENE_FILE_SECTION_TEXTURES;
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
			case SceneSection::SCENE_FILE_SECTION_BGCOLOR:
				ParseBGColor(line);
				break;
			case SceneSection::SCENE_FILE_SECTION_TEXTURES:
				ParseTextures(line);
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

	readFile.close();	
}

void Scene::Unload() {
	if (bgInstance) {
		bgInstance->Release();
	}

	for (GameObject* object : objects) {
		if (object) {
			object->Release();
		}
	}
	
	if (marioInstance) {
		marioInstance->Release();
	}

	if (cameraInstance) {
		cameraInstance->Release();
	}

	textureFiles.clear();
}

void Scene::UpdateCameraPosition() {
	if (marioInstance->GetPosition().x < 0.0f) {
		marioInstance->SetPosition(D3DXVECTOR3(0.0f, marioInstance->GetPosition().y, 0));
	}
	else if ((marioInstance->GetPosition().x + marioInstance->GetBoxWidth()) > sceneWidth) {
		marioInstance->SetPosition(D3DXVECTOR3(sceneWidth - marioInstance->GetBoxWidth(), marioInstance->GetPosition().y, 0));
	}

	D3DXVECTOR3 camPosition = marioInstance->GetPosition();
	camPosition.x -= Game::GetInstance()->GetScreenWidth() / 2;
	if (camPosition.x < 0) {
		camPosition.x = 0;
	}
	else if ((camPosition.x + Game::GetInstance()->GetScreenWidth()) > sceneWidth) {
		camPosition.x = sceneWidth - Game::GetInstance()->GetScreenWidth();
	}
	
	camPosition.y -= Game::GetInstance()->GetScreenHeight() / 2;	
	/*if (camPosition.y > 230) {
		camPosition.y = 230;
	}*/
	camPosition.y = 230;

	Camera::GetInstance()->SetPosition(camPosition);
}

void Scene::Update(DWORD delta) {
	std::vector<GameObject*> collidableObjects;
	for (GameObject* object : objects) {
		collidableObjects.push_back(object);
	}

	marioInstance->Update(delta, &collidableObjects);

	for (int i = 0; i < objects.size(); ++i) {
		objects.at(i)->Update(delta, &collidableObjects);

		if (dynamic_cast<Entity*>(objects.at(i))) {
			if (dynamic_cast<Entity*>(objects.at(i))->GetCurrentHitPoints() == 0) {
				//erase-remove idiom
				objects.erase(std::remove(objects.begin(), objects.end(), objects.at(i)), objects.end());
			}
		}
	}

	if (!marioInstance) {
		return;
	}

	UpdateCameraPosition();
}

void Scene::Render() {
	bgInstance->Render();
	
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
	
	marioInstance->Render();
}

void Scene::HandleStates(BYTE* states) {
	marioInstance->HandleStates(states);
}

void Scene::OnKeyDown(int keyCode) {
	marioInstance->OnKeyDown(keyCode);
}

void Scene::OnKeyUp(int keyCode) {
	marioInstance->OnKeyUp(keyCode);
}
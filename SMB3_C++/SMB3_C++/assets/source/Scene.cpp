#include "../headers/Scene.h"

Scene::Scene(int id, std::string path) {
	sceneID = id;
	filePath = path;

	bgInstance = new Background();
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

void Scene::ParseMapSize(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 2) {
		return;
	}

	sceneWidth = atoi(tokens.at(0).c_str());
	sceneHeight = atoi(tokens.at(1).c_str());
}

void Scene::ParseBGColor(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 3) {
		return;
	}

	unsigned int r = atoi(tokens.at(0).c_str());
	unsigned int g = atoi(tokens.at(1).c_str());
	unsigned int b = atoi(tokens.at(2).c_str());

	backgroundColor = D3DCOLOR_XRGB(r, g, b);
}

void Scene::ParseTextures(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 5) {
		return;
	}

	unsigned int texID = atoi(tokens.at(0).c_str());

	unsigned int r = atoi(tokens.at(2).c_str());
	unsigned int g = atoi(tokens.at(3).c_str());
	unsigned int b = atoi(tokens.at(4).c_str());

	textureFiles[texID] = std::pair<std::string, D3DCOLOR>(tokens.at(1), D3DCOLOR_XRGB(r, g, b));
}

void Scene::ParseBackground(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 7) {
		return;
	}

	RECT bound;
	bound.left = atoi(tokens.at(0).c_str());
	bound.top = atoi(tokens.at(1).c_str());
	bound.right = atoi(tokens.at(2).c_str()) + 1;
	bound.bottom = atoi(tokens.at(3).c_str()) + 1;

	int posX = atoi(tokens.at(4).c_str());
	int posY = atoi(tokens.at(5).c_str());
	D3DXVECTOR3 pos = D3DXVECTOR3(posX, posY, 0);

	int texID = atoi(tokens.at(6).c_str());
	bgInstance->LoadTexture(GetTexturePath(texID), GetTextureColorKey(texID));
	bgInstance->AddImage(bound, pos);
}

void Scene::ParseTilesData(std::string line) {

}

void Scene::ParseTileSprites(std::string line) {

}

void Scene::ParseEntityData(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 2) {
		return;
	}

	ObjectType objectID = static_cast<ObjectType>(atoi(tokens.at(0).c_str()));
	switch (objectID) {
		case ObjectType::OBJECT_TYPE_MARIO:
			marioInstance = Mario::GetInstance();
			marioInstance->ParseData(tokens.at(1));
			break;
		case ObjectType::OBJECT_TYPE_GOOMBA:

			break;
	}
}

void Scene::ParseWorldCoords(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 3) {
		return;
	}

	ObjectType objectID = static_cast<ObjectType>(atoi(tokens.at(0).c_str()));
	switch (objectID) {
		case ObjectType::OBJECT_TYPE_MARIO:
			int posX = atoi(tokens.at(1).c_str());
			int posY = atoi(tokens.at(2).c_str());

			D3DXVECTOR3 position(posX, posY, 0);

			marioInstance = Mario::GetInstance();
			marioInstance->SetPosition(position);
			break;
	}
}

void Scene::Load() {
	std::ifstream readFile;
	readFile.open(filePath, std::ios::in);

	if (!readFile.is_open()) {
		OutputDebugStringA("Failed to read file\n");
		return;
	}

	SceneSection section = SceneSection::SCENE_FILE_SECTION_UNKNOWN;

	char str[MAX_FILE_LINE];
	while (readFile.getline(str, MAX_FILE_LINE)) {
		std::string line(str);

		if (line[0] == '#' || line.empty()) {
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

		if (line == "[TILES_DATA") {
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

	/*cameraInstance = Camera::GetInstance();
	cameraInstance->SetPosition(D3DXVECTOR3(Game::GetInstance()->GetScreenWidth() / 2, Game::GetInstance()->GetScreenHeight() / 2, 0));*/
}

void Scene::Unload() {
	if (bgInstance) {
		bgInstance->Release();
	}

	for (Entity* entity : entities) {
		if (entity) {
			entity->Release();
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

void Scene::Update(DWORD delta) {	
	for (Entity* entity : entities) {
		entity->Update(delta);
	}

	marioInstance = Mario::GetInstance();
	marioInstance->Update(delta);

	D3DXVECTOR3 camPosition = marioInstance->GetPosition();
	camPosition.x -= Game::GetInstance()->GetScreenWidth() / 2;
	camPosition.y -= Game::GetInstance()->GetScreenHeight() / 2;
	//camPosition.y = 0;

	Camera::GetInstance()->SetPosition(camPosition);
}

void Scene::Render() {
	bgInstance->DrawBackground();
	
	for (Entity* entity : entities) {
		entity->Render();
	}

	marioInstance = Mario::GetInstance();
	marioInstance->Render();
}

void Scene::OnKeyDown(int keyCode) {
	Mario::GetInstance()->OnKeyDown(keyCode);
}

void Scene::OnKeyUp(int keyCode) {

}
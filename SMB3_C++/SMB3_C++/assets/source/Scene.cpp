#include "../headers/Scene.h"

Scene::Scene(int id, std::string path) {
	sceneID = id;
	filePath = path;
}

Scene::~Scene() {}

D3DCOLOR Scene::GetBGColor() {
	return backgroundColor;
}

D3DCOLOR Scene::GetTextureColorKey(int id) {
	return textureFiles[id].second;
}

std::string Scene::GetTexturePath(int id) {
	return textureFiles[id].first;
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

void Scene::ParseEntityData(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 2) {
		return;
	}

	int entityID = atoi(tokens.at(0).c_str());
	switch (entityID) {
		case 0:
			marioInstance = Mario::GetInstance();
			marioInstance->ParseData(tokens.at(1));
			break;
	}
}

void Scene::ParseWorldCoords(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 3) {
		return;
	}

	int entityID = atoi(tokens.at(0).c_str());
	switch (entityID) {
		case 0:
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

		switch (section) {
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
		}
	}

	readFile.close();
}

void Scene::Unload() {
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
	marioInstance = Mario::GetInstance();
	marioInstance->Update(delta);

	for (Entity* entity : entities) {
		entity->Update(delta);
	}
}

void Scene::Render() {
	marioInstance = Mario::GetInstance();
	marioInstance->Render();

	for (Entity* entity : entities) {
		entity->Render();
	}
}

void Scene::OnKeyDown(int keyCode) {
	switch (keyCode) {
		case DIK_A:
			OutputDebugStringA("A key pressed\n");
			break;
		case DIK_S:
			OutputDebugStringA("S key pressed\n");
			break;
		case DIK_D:
			OutputDebugStringA("D key pressed\n");
			break;
		case DIK_W:
			OutputDebugStringA("W key pressed\n");
			break;
		case DIK_SPACE:
			OutputDebugStringA("Space key pressed\n");
			break;
	}
}

void Scene::OnKeyUp(int keyCode) {

}
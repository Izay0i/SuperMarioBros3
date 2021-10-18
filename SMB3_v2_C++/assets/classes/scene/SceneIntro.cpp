#include "Scene.h"
#include "SceneIntro.h"

void SceneIntro::_IntroScript() {

}

SceneIntro::SceneIntro(SceneType sceneID, std::string path) : Scene(sceneID, path) {}

SceneIntro::~SceneIntro() {}

void SceneIntro::OnKeyDown(int keyCode) {

}

void SceneIntro::Update(DWORD deltaTime) {
	_IntroScript();
	
	for (unsigned int i = 0; i < _entities.size(); ++i) {
		Entity* entity = _entities.at(i);
		entity->Update(deltaTime, &_entities, &_tiles);
	}
}

void SceneIntro::Render() {
	_background->Render();

	for (unsigned int i = 0; i < _entities.size(); ++i) {
		_entities.at(i)->Render();
	}
}

void SceneIntro::Release() {
	char debug[100];
	sprintf_s(debug, "[SCENE] Unloading scene with ID: %d\n", _sceneID);
	OutputDebugStringA(debug);

	if (_background != nullptr) {
		_background->Release();
		delete _background;
	}

	for (auto& tile : _tiles) {
		tile->Release();
		delete tile;
	}
	_tiles.clear();

	for (unsigned int i = 0; i < _entities.size(); ++i) {
		_entities.at(i)->Release();
		delete _entities.at(i);
	}
	_entities.clear();

	for (auto& texture : _textureMap) {
		texture.second->Release();
		delete texture.second;
	}
	_textureMap.clear();

	if (_cameraInstance != nullptr) {
		_cameraInstance->Release();
	}

	sprintf_s(debug, "[SCENE] Unloaded scene with ID: %d\n", _sceneID);
	OutputDebugStringA(debug);
}
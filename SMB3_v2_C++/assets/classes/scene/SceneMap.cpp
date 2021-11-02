#include "../SceneManager.h"
#include "Scene.h"
#include "SceneMap.h"
#include "../audio/AudioService.h"

SceneMap::SceneMap(SceneType sceneID, std::string path) : Scene(sceneID, path) {
	_toSceneTime = 1200;
}

SceneMap::~SceneMap() {}

void SceneMap::HandleStates() {
	_player->HandleStates();
}

void SceneMap::OnKeyDown(int keyCode) {
	switch (keyCode) {
		case DIK_1:
			_player->SetHealth(1);
			break;
		case DIK_2:
			_player->SetHealth(2);
			break;
		case DIK_3:
			_player->SetHealth(3);
			break;
		case DIK_4:
			_player->SetHealth(4);
			break;
		case DIK_W:
		case DIK_A:
		case DIK_S:
		case DIK_D:
			AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_MAPMOVE);
			break;
		case DIK_K:
			if (_player->GetNextSceneID() != 0) {
				if (!IsTransitioningToScene()) {
					StartToSceneTimer();

					AudioService::GetAudio().StopAll();
					AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_LEVELSTART);
				}
			}
			break;
	}

	_player->OnKeyDownMap(keyCode);
}

void SceneMap::LoadScene() {
	Scene::LoadScene();
	
	_player->isInMap = true;

	for (auto it = _entities.begin(); it != _entities.end(); ++it) {
		Entity* entity = *it;
		entity->SetGravity(0.0f);
	}

	AudioService::GetAudio().PlayAudio(AudioType::AUDIO_TYPE_MAP, true);
}

void SceneMap::Update(DWORD deltaTime) {
	if (_player == nullptr) {
		char debug[100];
		sprintf_s(debug, "[SCENE] No player loaded in, scene ID: %d\n", _sceneID);
		OutputDebugStringA(debug);
		return;
	}
	
	for (auto it = _entities.begin(); it != _entities.end(); ++it) {
		Entity* entity = *it;
		entity->Update(deltaTime, &_entities, &_tiles, _grid);
	}
	std::sort(_entities.begin(), _entities.end(), Entity::CompareRenderPriority);

	_hud->Update(_sceneTime);
	_hud->SetPosition({
		_cameraInstance->GetPosition().x + 134.0f,
		_cameraInstance->GetPosition().y + 177.0f 
		}
	);

	if (IsTransitioningToScene()) {
		_player->SetVelocity({ 0.0f, 0.0f });

		if (GetTickCount64() - _toSceneStart > _toSceneTime) {
			_toSceneStart = 0;
			SceneManager::GetInstance()->ChangeScene(_player->GetNextSceneID());
		}
	}
}

void SceneMap::Render() {
	_background->Render();

	for (auto it = _entities.begin(); it != _entities.end(); ++it) {
		(*it)->Render();
	}

	_hud->Render();
}

void SceneMap::Release() {
	char debug[100];
	sprintf_s(debug, "[SCENE] Unloading scene with ID: %d\n", _sceneID);
	OutputDebugStringA(debug);

	AudioService::GetAudio().StopAll();

	_background->Release();
	delete _background;

	_hud->Release();
	delete _hud;

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
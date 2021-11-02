#include "../Game.h"
#include "../SceneManager.h"
#include "Scene.h"
#include "ScenePlay.h"
#include "../EntityList.h"
#include "../audio/AudioService.h"

ScenePlay::ScenePlay(SceneType sceneID, std::string path) : Scene(sceneID, path) {}

ScenePlay::~ScenePlay() {}

void ScenePlay::HandleStates() {
	_player->HandleStates();
}

void ScenePlay::OnKeyUp(int keyCode) {
	_player->OnKeyUpGame(keyCode);
}

void ScenePlay::OnKeyDown(int keyCode) {
	switch (keyCode) {
		case DIK_1:
			_player->SetHealth(1);
			break;
		case DIK_2:
			if (_player->GetHealth() == 1) {
				_player->SetPosition({ _player->GetPosition().x, _player->GetPosition().y - _player->GetBoxHeight(1) });
			}
			_player->SetHealth(2);
			break;
		case DIK_3:
			if (_player->GetHealth() == 1) {
				_player->SetPosition({ _player->GetPosition().x, _player->GetPosition().y - _player->GetBoxHeight(1) });
			}
			_player->SetHealth(3);
			break;
		case DIK_4:
			if (_player->GetHealth() == 1) {
				_player->SetPosition({ _player->GetPosition().x, _player->GetPosition().y - _player->GetBoxHeight(1) });
			}
			_player->SetHealth(4);
			break;
	}

	_player->OnKeyDownGame(keyCode);
}

void ScenePlay::LoadScene() {
	Scene::LoadScene();

	AudioService::GetAudio().PlayAudio(static_cast<AudioType>(_mainThemeID), true);
}

void ScenePlay::UpdateCameraPosition() {
	unsigned int index = _player->WentIntoPipe();
	RECTF cameraBound = _cameraInstance->GetCameraBound(index);
	_player->SetUpVector(_cameraInstance->GetUpVector(index));
	D3DXVECTOR2 cameraPosition = _cameraInstance->GetPosition();

	if (!_player->TriggeredStageEnd() && !_player->IsInPipe()) {
		if (_player->GetPosition().x < cameraPosition.x) {
			_player->SetPosition({ cameraPosition.x, _player->GetPosition().y });
		}
		else if (_player->GetPosition().x + _player->GetBoxWidth() > _sceneWidth) {
			_player->SetPosition({ _sceneWidth - _player->GetBoxWidth(), _player->GetPosition().y });
		}

		if (_player->GetPosition().y < cameraPosition.y) {
			_player->SetPosition({ _player->GetPosition().x, cameraPosition.y });
		}
	}

	cameraPosition = _player->GetPosition();
	cameraPosition.x -= Game::GetInstance()->GetWindowWidth() / 2.25f;
	if (cameraPosition.x < cameraBound.left) {
		cameraPosition.x = cameraBound.left;
	}
	else if (cameraPosition.x + Game::GetInstance()->GetWindowWidth() > cameraBound.right) {
		cameraPosition.x = cameraBound.right - Game::GetInstance()->GetWindowWidth();
	}

	cameraPosition.y -= Game::GetInstance()->GetWindowHeight() / 2.25f;
	if (_player->WentIntoPipe() || _player->IsFlying() || _player->GetPosition().y < _sceneHeight * 0.3f) {
		if (cameraPosition.y < cameraBound.top) {
			cameraPosition.y = cameraBound.top;
		}
		else if (cameraPosition.y + Game::GetInstance()->GetWindowHeight() > cameraBound.bottom) {
			cameraPosition.y = cameraBound.bottom - Game::GetInstance()->GetWindowHeight();
		}
	}
	else {
		cameraPosition.y = cameraBound.bottom - Game::GetInstance()->GetWindowHeight();
	}

	_cameraInstance->SetPosition(cameraPosition);
}

void ScenePlay::Update(DWORD deltaTime) {
	if (_player == nullptr) {
		char debug[100];
		sprintf_s(debug, "[SCENE] No player loaded in, scene ID: %d\n", _sceneID);
		OutputDebugStringA(debug);
		return;
	}
	
	if (_player->GetHealth() > 0 && !_player->TriggeredStageEnd()) {
		if (_sceneTime > 0 && GetTickCount64() % 1000 == 0) {
			--_sceneTime;
		}
	}

	if (_player->GetHealth() == 0 || _player->IsInvulnerable()) {
		_player->Update(deltaTime, &_entities, &_tiles, _grid);
	}
	else if (_player->GetHealth() > 0 && !_player->IsInvulnerable()) {
		//Range-based loop, for_each, iterators will all be invalidated if an element is either removed or inserted
		//And the container has to do a reallocation
		for (unsigned int i = 0; i < _entities.size(); ++i) {
			Entity* entity = _entities.at(i);
			entity->SetActive(_IsEntityInViewport(entity, _cameraInstance->GetViewport()));
			entity->Update(deltaTime, &_entities, &_tiles, _grid);

			//Entities events
			switch (entity->GetObjectType()) {
				case GameObject::GameObjectType::GAMEOBJECT_TYPE_PARAGOOMBA:
					{
						Paragoomba* paragoomba = dynamic_cast<Paragoomba*>(entity);
						if (paragoomba->IsWalking() && paragoomba->GetHealth() == 2) {
							//Mario is on the right side
							if (paragoomba->GetPosition().x - _player->GetPosition().x < 0.0f) {
								paragoomba->SetNormal({ -1.0f, 0.0f });
							}
							else {
								paragoomba->SetNormal({ 1.0f, 0.0f });
							}
						}
					}
					break;
				case GameObject::GameObjectType::GAMEOBJECT_TYPE_KOOPA:
				case GameObject::GameObjectType::GAMEOBJECT_TYPE_PARAKOOPA:
					{
						Koopa* koopa = dynamic_cast<Koopa*>(entity);
						if (koopa->GetHealth() == 2) {
							//Mario is on the right side
							if (koopa->GetPosition().x - _player->GetPosition().x < 0.0f) {
								koopa->SetNormal({ -1.0f, 0.0f });
							}
							else {
								koopa->SetNormal({ 1.0f, 0.0f });
							}
						}
					}
					break;
				case GameObject::GameObjectType::GAMEOBJECT_TYPE_PIRANHAPLANT:
				case GameObject::GameObjectType::GAMEOBJECT_TYPE_VENUSPLANT:
					{
						PiranaPlant* piranaPlant = dynamic_cast<PiranaPlant*>(entity);
						piranaPlant->ComparePlayerPosToSelf(_player->GetPosition());
						//Mario is on the right side
						if (piranaPlant->GetPosition().x - _player->GetPosition().x < 0.0f) {
							piranaPlant->SetScale({ -1.0f, piranaPlant->GetScale().y });
						}
						else {
							piranaPlant->SetScale({ 1.0f, piranaPlant->GetScale().y });
						}

						//Mario is below
						if (piranaPlant->GetPosition().y - _player->GetPosition().y < 0.0f) {
							piranaPlant->SetNormal({ -1.0f, piranaPlant->GetNormal().y });
						}
						else {
							piranaPlant->SetNormal({ 1.0f, piranaPlant->GetNormal().y });
						}
					}
					break;
				case GameObject::GameObjectType::GAMEOBJECT_TYPE_TAIL:
					{
						Tail* tail = dynamic_cast<Tail*>(entity);
						tail->SetPosition({
							_player->GetPosition().x, 
							_player->IsAttacking() ? _player->GetPosition().y + 2.0f : 0.0f
							}
						);
					}
					break;
				case GameObject::GameObjectType::GAMEOBJECT_TYPE_REDMUSHROOM:
				case GameObject::GameObjectType::GAMEOBJECT_TYPE_GREENMUSHROOM:
					{
						Mushroom* mushroom = dynamic_cast<Mushroom*>(entity);
						if (mushroom->IsEmerging()) {
							//Mario is on the right side
							if (mushroom->GetPosition().x - _player->GetPosition().x < 0.0f) {
								mushroom->SetNormal({ 1.0f, 1.0f });
							}
							else {
								mushroom->SetNormal({ -1.0f, 1.0f });
							}
						}
					}
					break;
				case GameObject::GameObjectType::GAMEOBJECT_TYPE_QUESTIONBLOCK:
					{
						QuestionBlock* questionBlock = dynamic_cast<QuestionBlock*>(entity);
						if (questionBlock->tookDamage) {
							AddEntityToScene(questionBlock->SpawnItem(_player->GetHealth()));
						}
					}
					break;
				case GameObject::GameObjectType::GAMEOBJECT_TYPE_SHINYBRICK:
					{
						ShinyBrick* shinyBrick = dynamic_cast<ShinyBrick*>(entity);
						if (shinyBrick->tookDamage) {
							AddEntityToScene(shinyBrick->SpawnItem());
						}
						else if (shinyBrick->GetHealth() == -1) {
							//Top left
							auto debris = shinyBrick->SpawnDebris();
							debris->SetVelocity({ -0.08f, -0.28f });
							AddEntityToScene(debris);
							//Top right
							debris = shinyBrick->SpawnDebris();
							debris->SetScale({ -1.0f, 1.0f });
							debris->SetVelocity({ 0.08f, -0.28f });
							AddEntityToScene(debris);
							//Bottom left
							debris = shinyBrick->SpawnDebris();
							debris->SetVelocity({ -0.08f, -0.18f });
							debris->SetPosition({ debris->GetPosition().x, debris->GetPosition().y + 10.0f });
							AddEntityToScene(debris);
							//Bottom right
							debris = shinyBrick->SpawnDebris();
							debris->SetScale({ -1.0f, 1.0f });
							debris->SetVelocity({ 0.08f, -0.18f });
							debris->SetPosition({ debris->GetPosition().x, debris->GetPosition().y + 10.0f });
							AddEntityToScene(debris);
						}
					}
					break;
			}

			if (entity->tookDamage) {
				_scorePopUp->GetEntity(entity);
				_scorePopUp->SetPosition(entity->GetPosition());
				_scorePopUp->StartFloatTimer();

				entity->tookDamage = false;
			}

			if (_grid != nullptr) {
				Cell* newCell = _grid->GetCell(entity->GetPosition());
				if (newCell != entity->ownerCell) {
					_grid->RemoveEntity(entity);
					_grid->AddEntity(entity, newCell);
				}
			}

			if (!_IsEntityAliveAndIB(entity)) {
				if (_grid != nullptr) {
					_grid->RemoveEntity(entity);
				}
				delete entity;

				_entities.erase(std::remove(_entities.begin(), _entities.end(), entity), _entities.end());
			}
		}
		std::sort(_entities.begin(), _entities.end(), Entity::CompareRenderPriority);
	}

	UpdateCameraPosition();

	_scorePopUp->Update(deltaTime);

	_hud->Update(_sceneTime);
	_hud->SetPosition({
		_cameraInstance->GetPosition().x + 134.0f,
		_cameraInstance->GetPosition().y + 161.0f 
		}
	);

	if (_player->TriggeredStageEnd() || _player->GetHealth() == 0 || _sceneTime == 0) {
		//Warp back to map				
		if (!IsTransitioningToScene()) {
			StartToSceneTimer();
		}

		if (IsTransitioningToScene() && GetTickCount64() - _toSceneStart > _toSceneTime) {
			_toSceneStart = 0;
			SceneManager::GetInstance()->ChangeScene(static_cast<unsigned int>(SceneType::SCENE_TYPE_MAP));
			return;
		}
	}
}

void ScenePlay::Render() {
	_background->Render();

	for (unsigned int i = 0; i < _entities.size(); ++i) {
		Entity* entity = _entities.at(i);
		
		if (!entity->IsActive()) {
			continue;
		}

		entity->Render();
	}
	
	_scorePopUp->Render();

	_hud->Render();
}

void ScenePlay::Release() {
	char debug[100];
	sprintf_s(debug, "[SCENE] Unloading scene with ID: %d\n", _sceneID);
	OutputDebugStringA(debug);

	AudioService::GetAudio().StopAll();

	_background->Release();
	delete _background;

	_hud->Release();
	delete _hud;

	_scorePopUp->Release();
	delete _scorePopUp;

	if (_grid != nullptr) {
		_grid->Release();
		delete _grid;
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
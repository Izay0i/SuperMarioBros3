#include "../SceneManager.h"
#include "Scene.h"
#include "SceneIntro.h"
#include "../EntityList.h"

void SceneIntro::_IntroScript() {
	//I can't think of a clever way to make a script parser that supports function delegation, prop list and such
	//So I'll be hardcoding every single value, every single action I've observed, in milliseconds
	//Oh yeah it's YanDev time
	//This is ugly
	//I can't believe I'm doing this but I have no choice
	//Time is money
	//Please don't ever try to replicate this code
	DWORD time = static_cast<DWORD>(GetTickCount64()) - _toSceneStart;
	if (time > _sceneTime) {
		return;
	}

	if (time >= 0 && time < 9875) {
		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_GOOMBA) {
				Goomba* goomba = dynamic_cast<Goomba*>(entity);
				goomba->SetNormal({ 0.0f, 1.0f });
			}
		}
	}
	if (time >= 0 && time < 8000) {
		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_RMUSHROOM) {
				Mushroom* mushroom = dynamic_cast<Mushroom*>(entity);
				mushroom->SetVelocity({ 0.0f, mushroom->GetVelocity().y });
			}
		}
	}

	if (time >= 0 && time < 1758) {
		_propMario->SetScale({ 0.0, 0.0f });
		
		_propLuigi->SetScale({ 0.0f, 0.0f });
	}
	if (time >= 1758 && time < 2312) {
		_propMario->SetScale({ 1.0f, 1.0f });
		
		_propLuigi->SetScale({ -1.0f, 1.0f });
	}
	if (time >= 2312 && time < 2937) {
		_propMario->animationName = "BigRun";
		
		_propLuigi->animationName = "BigRun";
	}
	if (time >= 2937 && time < 3500) {
		_propMario->MoveLeft();
		
		_propLuigi->MoveRight();
	}
	if (time >= 3500 && time < 4000) {
		_propLuigi->animationName = "BigJump";
		_propLuigi->SetJumpSpeed(0.16f);
		_propLuigi->Jump();
	}
	if (time >= 4000 && time < 4437) {
		_propLuigi->animationName = "BigFall";
	}
	if (time >= 4237 && time < 4737) {
		_propMario->animationName = "BigCrouch";
		_propMario->SetVelocity({});

		_propLuigi->animationName = "BigJump";
		_propLuigi->SetJumpSpeed(0.17f);
		_propLuigi->Jump();
	}
	if (time >= 4737 && time < 5000) {
		_propLuigi->animationName = "BigFall";
	}
	if (time >= 4837 && time < 5000) {
		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_LOGO) {
				GameLogo* gameLogo = dynamic_cast<GameLogo*>(entity);
				gameLogo->StartFallDownTimer();
			}
		}
	}
	if (time >= 5000 && time < 7812) {
		_propMario->animationName = "BigIdle";
		
		_propLuigi->animationName = "BigRun";
	}
	if (time >= 6000 && time < 7812) {
		const D3DXCOLOR BG_COLOR = { 252 / 255.0f, 216 / 255.0f, 168 / 255.0f, 1.0f };
		_backgroundColor = BG_COLOR;

		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_SELECT) {
				continue;
			}
			entity->SetScale({ 1.0f, 1.0f });
		}
	}
	if (time >= 7812 && time < 8062) {
		_propMario->animationName = "BigIntroImpact";

		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPKOOPASHELL) {
				PropKoopaShell* shell = dynamic_cast<PropKoopaShell*>(entity);
				shell->SetVelocity({ shell->GetVelocity().x, -0.04f });
				shell->animationName = "GreenShellSpin";
			}
		}
	}
	if (time >= 8062 && time < 8250) {
		_propMario->animationName = "BigIdle";

		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPKOOPASHELL) {
				PropKoopaShell* shell = dynamic_cast<PropKoopaShell*>(entity);
				shell->SetVelocity({ -0.02f, shell->GetVelocity().y });
				shell->animationName = "GreenShellIdle";
			}
		}
	}
	if (time >= 8250 && time < 9000) {
		_propMario->animationName = "BigIntroLookUp";
	}
	if (time >= 9000 && time < 9875) {
		_propMario->animationName = "BigJump";
		_propMario->SetJumpSpeed(0.1f);
		_propMario->Jump();

		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPKOOPASHELL) {
				PropKoopaShell* shell = dynamic_cast<PropKoopaShell*>(entity);
				shell->SetVelocity({ 0.0f, shell->GetVelocity().y });
			}
		}
	}
	if (time >= 9700 && time < 9850) {
		_propMario->animationName = "SmokePuff";

		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_LEAF) {
				Leaf* leaf = dynamic_cast<Leaf*>(entity);
				leaf->SetPosition({ 500.0f, 500.0f });
			}
		}
	}
	if (time >= 9875 && time < 10937) {
		_propMario->animationName = "RacWagFall";
		_propMario->SlowFall();
		_propMario->SetRunSpeed(0.05f);
		_propMario->MoveLeft();

		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_GOOMBA) {
				Goomba* goomba = dynamic_cast<Goomba*>(entity);
				goomba->SetNormal({ 1.0f, 1.0f });
			}
		}
	}
	if (time >= 10937 && time < 11187) {
		_propMario->animationName = "RacJump";
		_propMario->Jump();

		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_GOOMBA) {
				Goomba* goomba = dynamic_cast<Goomba*>(entity);
				goomba->TakeDamage();
			}
		}
	}
	if (time >= 11187 && time < 11375) {
		_propMario->animationName = "RacFall";

		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_GOOMBA) {
				Goomba* goomba = dynamic_cast<Goomba*>(entity);
				goomba->SetPosition({ 500.0f, 500.0f });
			}
		}
	}
	if (time >= 11375 && time < 11562) {
		_propMario->animationName = "RacSkid";
		_propMario->SetScale({ -1.0f, 1.0f });
	}
	if (time >= 11562 && time < 12312) {
		_propMario->animationName = "RacRun";
		_propMario->SetRunSpeed(0.068f);
		_propMario->MoveRight();
	}
	if (time >= 12212 && time < 12500) {
		_propMario->animationName = "RacKick";

		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPKOOPASHELL) {
				PropKoopaShell* shell = dynamic_cast<PropKoopaShell*>(entity);
				shell->SetVelocity({ 0.18f, shell->GetVelocity().y });
				shell->animationName = "GreenShellSpin";
			}
		}
	}
	if (time >= 12500 && time < 13875) {
		_propMario->animationName = "RacRun";
	}
	if (time >= 13875 && time < 14000) {
		_propMario->animationName = "RacIdle";
		_propMario->SetVelocity({ 0.0f, 0.0f });
	}
	if (time >= 14000 && time < 14562) {
		_propLuigi->animationName = "BigHoldRun";
		_propLuigi->SetRunSpeed(0.042f);
		_propLuigi->SetScale({ 1.0f, 1.0f });
		_propLuigi->MoveLeft();

		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPKOOPASHELL) {
				PropKoopaShell* shell = dynamic_cast<PropKoopaShell*>(entity);
				shell->SetVelocity({ 0.0f, 0.0f });
				shell->animationName = "GreenShellIdle";
				shell->SetPosition({ _propLuigi->GetPosition().x - 12.0f, _propLuigi->GetPosition().y });
			}
		}
	}
	if (time >= 14562 && time < 14875) {
		_propMario->animationName = "RacRun";
		_propMario->SetScale({ 1.0f, 1.0f });
		_propMario->SetRunSpeed(0.09f);
		_propMario->MoveLeft();
	}
	if (time >= 14875 && time < 15187) {
		_propLuigi->animationName = "BigHoldIdle";
		_propLuigi->SetVelocity({ 0.0f, 0.0f });
	}
	if (time >= 15187 && time < 15400) {
		_propLuigi->animationName = "BigKick";

		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPKOOPASHELL) {
				PropKoopaShell* shell = dynamic_cast<PropKoopaShell*>(entity);
				shell->SetVelocity({ -0.18f, shell->GetVelocity().y });
				shell->animationName = "GreenShellSpin";
			}
		}
	}
	if (time >= 15400 && time < 15625) {
		_propLuigi->animationName = "BigIdle";
	}
	if (time >= 15625 && time < 15875) {
		_propMario->animationName = "RacJump";
		_propMario->Jump();
	}
	if (time >= 15875 && time < 15937) {
		_propMario->animationName = "RacFall";
	}
	if (time >= 15937 && time < 16175) {
		_propMario->animationName = "RacJump";
		_propMario->SetRunSpeed(0.02f);
		_propMario->SetJumpSpeed(0.01f);
		_propMario->Jump();
	}
	if (time >= 16175 && time < 16375) {
		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPKOOPASHELL) {
				PropKoopaShell* shell = dynamic_cast<PropKoopaShell*>(entity);
				shell->SetVelocity({ 0.0f, 0.0f });
				shell->animationName = "GreenShellIdle";
			}
		}
	}
	if (time >= 16375 && time < 16625) {
		_propMario->animationName = "RacIdle";
		_propMario->MoveLeft();
	}
	if (time >= 16625 && time < 16725) {
		_propMario->animationName = "RacSkid";
		_propMario->SetScale({ -1.0f, 1.0f });
	}
	if (time >= 16725 && time < 17683) {
		_propMario->animationName = "RacHoldRun";
		_propMario->SetRunSpeed(0.065f);
		_propMario->MoveRight();

		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPKOOPASHELL) {
				PropKoopaShell* shell = dynamic_cast<PropKoopaShell*>(entity);
				shell->SetPosition({ _propMario->GetPosition().x + 12.0f, _propMario->GetPosition().y });
			}
		}
	}
	if (time >= 17683 && time < 17783) {
		_propMario->animationName = "RacKick";
		
		_propLuigi->animationName = "BigRun";
		_propLuigi->SetScale({ -1.0f, 1.0f });
		_propLuigi->SetRunSpeed(0.09f);
		_propLuigi->MoveRight();

		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPKOOPASHELL) {
				PropKoopaShell* shell = dynamic_cast<PropKoopaShell*>(entity);
				shell->SetVelocity({ 0.18f, shell->GetVelocity().y });
				shell->animationName = "GreenShellSpin";
			}
		}
	}
	if (time >= 17783 && time < 17875) {
		//_propLuigi->SetVelocity({ 0.0f, 0.0f });
	}
	if (time >= 17875 && time < 18187) {
		_propMario->animationName = "RacRun";
		_propMario->SetRunSpeed(0.04f);
		_propMario->MoveRight();
	}
	if (time >= 18187 && time < 19875) {
		_propMario->animationName = "RacIdle";
		_propMario->SetVelocity({ 0.0f, 0.0f });
	}
	if (time >= 19575 && time < 19675) {
		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPKOOPASHELL) {
				PropKoopaShell* shell = dynamic_cast<PropKoopaShell*>(entity);
				shell->SetPosition({ 16.0f, shell->GetPosition().y });
			}
		}
	}
	if (time >= 19675 && time < 19875) {
		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPKOOPASHELL) {
				PropKoopaShell* shell = dynamic_cast<PropKoopaShell*>(entity);
				shell->SetVelocity({ 0.18f, shell->GetVelocity().y });
			}
		}
	}
	if (time >= 19975 && time < 20875) {
		_propMario->animationName = "ShrinkDown";
	}
	if (time >= 20875 && time < 21250) {
		_propMario->SetHealth(1);
		_propMario->animationName = "Idle";
	}
	if (time >= 21000 && time < 21250) {
		_propMario->SetScale({ 1.0f, 1.0f });
	}
	if (time >= 21250 && time < 22125) {
		_propMario->animationName = "Run";
		_propMario->SetScale({ -1.0f, 1.0f });
		_propMario->SetRunSpeed(0.11f);
		_propMario->MoveRight();
	}
	if (time >= 22125 && time < 22750) {
		_propMario->animationName = "Skid";
		_propMario->SetScale({ 1.0f, 1.0f });
		_propMario->SetVelocity({ 0.01f, 0.0f });
	}
	if (time >= 22750 && time < 23687) {
		_propMario->animationName = "Run";
		_propMario->SetRunSpeed(0.028f);
		_propMario->MoveLeft();
	}
	if (time >= 24687 && time < 26437) {
		_propMario->animationName = "Idle";
		_propMario->SetVelocity({ 0.0f, 0.0f });
	}
	if (time >= 26437 && time < 28437) {
		_propMario->SetRenderPriority(4);
		_propMario->animationName = "Run";
		_propMario->SetScale({ -1.0f, 1.0f });
		_propMario->SetRunSpeed(0.09f);
		_propMario->MoveRight();
	}
	if (time >= 28437 && time < _sceneTime) {
		_selectText->SetScale({ 1.0f, 1.0f });

		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			switch (entity->GetObjectType()) {
				case GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPNORMALKOOPA:
					entity->SetNormal({ 1.0f, 1.0f });
					break;
			}
		}
	}
	if (time >= 37000 && time < _sceneTime) {
		for (auto it = _entities.begin(); it != _entities.end(); ++it) {
			Entity* entity = *it;
			switch (entity->GetObjectType()) {
			case GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPFASTKOOPA:
				entity->SetNormal({ 1.0f, 1.0f });
				break;
			}
		}
	}
}

SceneIntro::SceneIntro(SceneType sceneID, std::string path) : Scene(sceneID, path) {}

SceneIntro::~SceneIntro() {}

void SceneIntro::OnKeyDown(int keyCode) {
	switch (keyCode) {
		case DIK_U:
			_selectText->isMultiplayer = !_selectText->isMultiplayer;
			break;
		case DIK_I:
			if (_selectText->GetScale() == D3DXVECTOR2(1.0f, 1.0f)) {
				SceneManager::GetInstance()->ChangeScene(static_cast<unsigned int>(SceneType::SCENE_TYPE_MAP));
			}
			break;
	}
}

void SceneIntro::LoadScene() {
	Scene::LoadScene();
	_toSceneStart = static_cast<DWORD>(GetTickCount64());

	for (auto it = _entities.begin(); it != _entities.end(); ++it) {
		Entity* entity = *it;
		switch (entity->GetObjectType()) {
			case GameObject::GameObjectType::GAMEOBJECT_TYPE_CURTAIN:
				continue;
			case GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPFASTKOOPA:
			case GameObject::GameObjectType::GAMEOBJECT_TYPE_PROPNORMALKOOPA:
				entity->SetNormal({ 0.0f, 1.0f });
				break;
		}
		entity->SetScale({ 0.0f, 0.0f });
	}
}

void SceneIntro::Update(DWORD deltaTime) {
	_IntroScript();
	
	for (auto it = _entities.begin(); it != _entities.end(); ++it) {
		Entity* entity = *it;
		entity->Update(deltaTime, &_entities, &_tiles);
	}
	std::sort(_entities.begin(), _entities.end(), Entity::CompareRenderPriority);
}

void SceneIntro::Render() {
	_background->Render();

	for (auto it = _entities.begin(); it != _entities.end(); ++it) {
		(*it)->Render();
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
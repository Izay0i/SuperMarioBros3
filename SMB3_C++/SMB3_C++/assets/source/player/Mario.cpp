#include "../../headers/player/Mario.h"

Entity* Mario::marioInstance = nullptr;
LPCWSTR Mario::texturePath = nullptr;
LPDIRECT3DTEXTURE9 Mario::texture = nullptr;
D3DCOLOR Mario::colorKey = D3DCOLOR_XRGB(0, 0, 0);

Mario::Mario() {
	//0 - dead
	//1 - small
	//2 - big
	//3 - fire
	//4 - racoon
	hitPoints = 3;

	scale = D3DXVECTOR2(-1.0f, 1.0f);
}

Mario* Mario::GetInstance() {
	if (!marioInstance) {
		marioInstance = new Mario;
	}

	return static_cast<Mario*>(marioInstance);
}

void Mario::LoadTexture() {
	if (!texture) {
		HRESULT hResult;
		D3DXIMAGE_INFO imageInfo;

		hResult = D3DXGetImageInfoFromFile(texturePath, &imageInfo);
		if (hResult != D3D_OK) {
			OutputDebugStringA("[MARIO] Failed to get image info\n");
			return;
		}

		hResult = D3DXCreateTextureFromFileEx(
			directDevice,
			texturePath,
			imageInfo.Width,
			imageInfo.Height,
			1,
			D3DUSAGE_DYNAMIC,
			D3DFMT_UNKNOWN,
			D3DPOOL_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			colorKey,
			&imageInfo,
			nullptr,
			&texture
		);

		if (hResult != D3D_OK) {
			OutputDebugStringA("Failed to create entity sprite from file\n");
			return;
		}
	}
}

RECTF Mario::GetBoundingBox(int id) const {
	RECTF bound;
	bound.left = position.x + 1;
	bound.top = position.y + 1;
	bound.right = position.x + hitBox.GetWidth(id);

	if (hitPoints == 1) {		
		bound.bottom = position.y + hitBox.GetHeight(id);
	}
	else {
		bound.bottom = position.y + hitBox.GetHeight(1);
	}

	return bound;
}

void Mario::ParseSprites(std::string line) {
	sprite.ParseSprites(line, texture, colorKey);
}

void Mario::ParseHitboxes(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 4) {
		return;
	}

	unsigned int left = atoi(tokens.at(0).c_str());
	unsigned int top = atoi(tokens.at(1).c_str());
	unsigned int right = atoi(tokens.at(2).c_str());
	unsigned int bottom = atoi(tokens.at(3).c_str());

	RECTF hitbox;
	hitbox.left = left;
	hitbox.top = top;
	hitbox.right = right;
	hitbox.bottom = bottom;

	this->hitBox.AddHitBox(hitbox);
}

void Mario::ParseData(std::string dataPath, std::string projPath, std::string texturePath, D3DCOLOR colorKey) {
	std::ifstream readFile;
	readFile.open(dataPath, std::ios::in);

	if (!readFile.is_open()) {
		OutputDebugStringA("Failed to read data\n");
		return;
	}

	fireballPath = projPath;
	texPath = texturePath;

	this->texturePath = Util::ToLPCWSTR(texturePath);
	this->colorKey = colorKey;

	LoadTexture();

	DataSection section = DataSection::DATA_SECTION_UNKNOWN;

	char str[MAX_FILE_LINE];
	while (readFile.getline(str, MAX_FILE_LINE)) {
		std::string line(str);

		if (line[0] == '#' || line.empty()) {
			continue;
		}

		if (line == "[SPRITES]") {
			section = DataSection::DATA_SECTION_SPRITES;
			continue;
		}

		if (line == "[HITBOXES]") {
			section = DataSection::DATA_SECTION_HITBOXES;
			continue;
		}

		switch (section) {
			case DataSection::DATA_SECTION_SPRITES:
				ParseSprites(line);
				break;
			case DataSection::DATA_SECTION_HITBOXES:
				ParseHitboxes(line);
				break;
		}
	}

	readFile.close();

	marioFSM = new MarioStateMachine(static_cast<Mario*>(marioInstance));
}

void Mario::HandleStates(BYTE* states) {
	if (Device::IsKeyDown(DIK_A) || Device::IsKeyDown(DIK_D)) {
		//GOTTA GO FAAAST
		if (acceleration < 1.79f) {
			acceleration += 0.01f;
		}
	}

	if (!Device::IsKeyDown(DIK_SPACE)) {
		gravity = 0.003f;
	}

	if (Device::IsKeyDown(DIK_A)) {
		//to flip sprite
		scale = D3DXVECTOR2(1.0f, 1.0f);
		velocity.x = -runSpeed * acceleration;
		normal.x = -1;
	}
	else if (Device::IsKeyDown(DIK_D)) {
		scale = D3DXVECTOR2(-1.0f, 1.0f);
		velocity.x = runSpeed * acceleration;
		normal.x = 1;
	}
	else {
		velocity.x = 0.0f;
		acceleration = 1.0f;
	}

	marioFSM->HandleStates(states);
}

void Mario::OnKeyDown(int keyCode) {
	switch (keyCode) {
		case DIK_SPACE:
			if (IsOnGround()) {
				gravity = 0.0012f;
				velocity.y = -jumpSpeed;
				isOnGround = false;
			}
			break;
		case DIK_J: //hold?
			OutputDebugStringA("Holding\n");
			break;
		case DIK_K: //spin or shoot
			if (hitPoints == 3 && !Device::IsKeyDown(DIK_S)) {
				SceneManager::GetInstance()->GetCurrentScene()->AddObjectToScene(SpawnFireball());
			}
			break;
	}
}

Fireball* Mario::SpawnFireball() {
	Fireball* fireball = new Fireball;
	fireball->SetObjectID(99);
	fireball->ParseData(fireballPath, texPath, colorKey);
	fireball->SetNormal(D3DXVECTOR3(normal.x, 0, 0));
	fireball->SetPosition(D3DXVECTOR3(position.x + normal.x * 16, position.y + 10, 0));
	return fireball;
}

void Mario::TakeDamage() {
	if (hitPoints == 1) {
		--hitPoints;
	}
	else {
		hitPoints = 1;
	}
}

void Mario::Update(DWORD delta, std::vector<GameObject*>* objects) {
	GameObject::Update(delta);
	
	velocity.y += gravity * delta;

	std::vector<LPCOLLISIONEVENT> collisionEvents, eventResults;
	collisionEvents.clear();

	if (hitPoints != 0) {
		CalcPotentialCollision(objects, collisionEvents);
	}

	if (collisionEvents.size() == 0) {
		position += distance;
	}
	else {
		D3DXVECTOR2 minTime;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 relativeDistance;

		FilterCollision(collisionEvents, eventResults, minTime, normal, relativeDistance);

		position.x += minTime.x * distance.x + normal.x * 0.1f;
		position.y += minTime.y * distance.y + normal.y * 0.1f;

		if (normal.x != 0.0f) {
			velocity.x = 0.0f;
		}

		if (normal.y != 0.0f) {
			velocity.y = 0.0f;
		}

		for (LPCOLLISIONEVENT result : eventResults) {
			LPCOLLISIONEVENT event = result;
			
			if (dynamic_cast<Goomba*>(event->object)) {
				Goomba* goomba = static_cast<Goomba*>(event->object);
				if (event->normal.y < 0.0f) {
					if (goomba->GetCurrentHitPoints() > 0) {
						velocity.y = -deflectSpeed;
					}
					goomba->TakeDamage();
				}
				else if (event->normal.x != 0.0f) {
					if (goomba->GetCurrentHitPoints() != 0) {
						if (Device::IsKeyDown(DIK_K) && hitPoints == 4) {
							goomba->TakeDamage();
						}
						else {
							TakeDamage();
							velocity.y = -dieflectSpeed;
						}
					}
				}
			}

			if (dynamic_cast<KoopaTroopa*>(event->object)) {
				KoopaTroopa* koopa = static_cast<KoopaTroopa*>(event->object);
				if (event->normal.y < 0.0f) {
					if (koopa->GetCurrentHitPoints() > 1) {
						velocity.y = -deflectSpeed;
					}
					koopa->TakeDamage();
					koopa->StartRetract();
					//follow the player
					koopa->SetNormal(
						D3DXVECTOR3(
							this->normal.x == event->normal.x ? -this->normal.x : this->normal.x,
							0,
							0
						)
					);
				}
				else if (event->normal.x != 0.0f) {
					if (koopa->GetCurrentHitPoints() == 2) {
						if (Device::IsKeyDown(DIK_K) && hitPoints == 4) {
							koopa->TakeDamage();
							koopa->StartRetract();
							koopa->SetNormal(
								D3DXVECTOR3(
									this->normal.x == event->normal.x ? -this->normal.x : this->normal.x,
									0,
									0
								)
							);
						}
						else {
							TakeDamage();
							velocity.y = -dieflectSpeed;
						}
					}
				}
			}

			if (event->normal.y == -1.0f) {
				isOnGround = true;
			}
		}
	}

	for (LPCOLLISIONEVENT event : collisionEvents) {
		delete event;
	}
}

void Mario::Render() {
	marioFSM->Render();
}

void Mario::Release() {
	if (marioInstance) {
		delete marioInstance;
		marioInstance = nullptr;
	}
}
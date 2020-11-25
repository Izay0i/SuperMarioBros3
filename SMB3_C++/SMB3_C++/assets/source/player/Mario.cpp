#include "../../headers/player/Mario.h"

Mario* Mario::marioInstance = nullptr;
LPCWSTR Mario::texturePath = nullptr;
LPDIRECT3DTEXTURE9 Mario::texture = nullptr;
D3DCOLOR Mario::colorKey = D3DCOLOR_XRGB(0, 0, 0);

Mario::Mario() {
	//0 - dead
	//1 - small
	//2 - big
	//3 - fire
	//4 - racoon
	hitPoints = 4;

	scale = D3DXVECTOR2(-1.0f, 1.0f);
}

Mario* Mario::GetInstance() {
	if (!marioInstance) {
		marioInstance = new Mario;
	}

	return marioInstance;
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
	bound.left = position.x;
	bound.top = position.y;
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

	float left = std::stof(tokens.at(0));
	float top = std::stof(tokens.at(1));
	float right = std::stof(tokens.at(2));
	float bottom = std::stof(tokens.at(3));

	RECTF hitbox;
	hitbox.left = left;
	hitbox.top = top;
	hitbox.right = right;
	hitbox.bottom = bottom;

	this->hitBox.AddHitBox(hitbox);
}

void Mario::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::string projPath) {
	std::ifstream readFile;
	readFile.open(dataPath, std::ios::in);

	if (!readFile.is_open()) {
		OutputDebugStringA("Failed to read data\n");
		return;
	}

	extraData = projPath;

	this->texturePath = Util::ToLPCWSTR(texturePath);
	this->colorKey = colorKey;

	LoadTexture();

	DataSection section = DataSection::DATA_SECTION_UNKNOWN;

	char str[MAX_FILE_LINE];
	while (readFile.getline(str, MAX_FILE_LINE)) {
		std::string line(str);

		if (line.empty() || line.front() == '#') {
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

	marioFSM = new MarioStateMachine(marioInstance);
}

void Mario::HandleStates(BYTE* states) {
	//for picking up koopa shells
	if (Device::IsKeyDown(DIK_J)) {
		isHolding = true;
	}
	else {
		isHolding = false;
	}
	
	//still haven't thought of a proper way to implement variable jump height yet
	if (Device::IsKeyDown(DIK_K)) {
		if (gravity > MAX_GRAVITY) {
			gravity -= 0.0005f;
		}		
	}
	else {
		if (gravity < 0.0025f) {
			gravity += 0.0005f;
		}
	}
	
	if (Device::IsKeyDown(DIK_A)) {
		//to flip sprite
		scale = D3DXVECTOR2(1.0f, 1.0f);
		velocity.x = -runSpeed * acceleration;
	}
	else if (Device::IsKeyDown(DIK_D)) {
		scale = D3DXVECTOR2(-1.0f, 1.0f);
		velocity.x = runSpeed * acceleration;
	}
	else {
		velocity.x = 0.0f;
	}

	marioFSM->HandleStates(states);
}

void Mario::OnKeyDown(int keyCode) {
	//Controls:
	//A: move left
	//D: move right
	//S: duck/crouch
	//J: B button - hold object, kick it's ass, wag your tail, shoot fireballs
	//K: A button - jump, high jump, slow fall
	switch (keyCode) {
		case DIK_A:
			normal.x = -1;
			break;
		case DIK_D:
			normal.x = 1;
			break;
		case DIK_J:
			if (hitPoints == 3 && !Device::IsKeyDown(DIK_S)) {
				SceneManager::GetInstance()->GetCurrentScene()->AddObjectToScene(SpawnFireball());
			}
			break;
		case DIK_K:
			//slow falling when in air
			if (!IsOnGround() && hitPoints == 4) {
				velocity.y *= 0.02f;
			}

			//unlimited jumping if Mario is Racoon
			if (acceleration >= ACCEL_THRESHOLD && hitPoints == 4) {
				velocity.y = -jumpSpeed;
				isOnGround = false;
			}

			//apply normal gravity if its just walking
			if (IsOnGround()) {
				velocity.y = -jumpSpeed;
				isOnGround = false;
			}
			break;
	}
}

Fireball* Mario::SpawnFireball() {
	Fireball* fireball = new Fireball;
	fireball->SetObjectID(99);	
	fireball->ParseData(extraData, Util::ToStr(texturePath), colorKey);
	fireball->SetNormal(D3DXVECTOR3(normal.x, 0, 0));
	fireball->SetPosition(D3DXVECTOR3(position.x, position.y + 10, 0));
	return fireball;
}

void Mario::TakeDamage() {
	if (hitPoints > 2) {
		hitPoints = 2;
	}
	else {
		--hitPoints;
	}
}

void Mario::Update(DWORD delta, std::vector<GameObject*>* objects) {	
	//stop moving if Mario dies
	if (hitPoints == 0) {
		velocity.x = 0;
	}
	
	GameObject::Update(delta);
	
	velocity.y += gravity * delta;

	if ((Device::IsKeyDown(DIK_A) || Device::IsKeyDown(DIK_D)) && Device::IsKeyDown(DIK_J)) {
		//GOTTA GO FAAAST
		if (acceleration < MAX_ACCEL) {
			acceleration += 0.01f;
		}
	}
	else {
		if (acceleration > 1.0f) {
			acceleration -= 0.01f;
		}
	}
	
	//float just a little bit longer when flying
	if (acceleration >= ACCEL_THRESHOLD) {
		gravity = 0.0008f;
	}

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

		position.x += minTime.x * distance.x + normal.x * 0.4f;
		position.y += minTime.y * distance.y + normal.y * 0.4f;

		if (normal.x != 0.0f) {
			velocity.x = 0.0f;
		}

		if (normal.y != 0.0f) {
			velocity.y = 0.0f;
		}

		for (LPCOLLISIONEVENT result : eventResults) {
			LPCOLLISIONEVENT event = result;
			
			if (dynamic_cast<Tiles*>(event->object) || 
				dynamic_cast<QuestionBlock*>(event->object) || 
				dynamic_cast<ShinyBrick*>(event->object)) 
			{
				if (event->normal.y == -1.0f) {
					isOnGround = true;
				}
				
			}

			if (dynamic_cast<Coin*>(event->object)) {
				//OutputDebugStringA("Coin\n");
			}

			//question block
			if (dynamic_cast<QuestionBlock*>(event->object)) {
				QuestionBlock* questionBlock = static_cast<QuestionBlock*>(event->object);
				if (event->normal.y > 0.0f) {
					questionBlock->TakeDamage();
				}
			}

			//pirana plant
			if (dynamic_cast<PiranaPlant*>(event->object)) {
				TakeDamage();
			}

			//goomba
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
						if (Device::IsKeyDown(DIK_J) && velocity.x == 0.0f) {
							//stand still
							if (hitPoints == 4) {
								goomba->TakeDamage();
							}
							else {
								TakeDamage();
								if (hitPoints > 0) {
									velocity.y = -deflectSpeed;
								}
								else {
									velocity.y -= dieflectSpeed;
								}
							}
						}
						else {
							TakeDamage();
							if (hitPoints > 0) {
								velocity.y = -deflectSpeed;
							}
							else {
								velocity.y -= dieflectSpeed;
							}
						}
					}
				}
			}

			//koopa troopa
			if (dynamic_cast<KoopaTroopa*>(event->object)) {
				KoopaTroopa* koopa = static_cast<KoopaTroopa*>(event->object);
				//on koopa
				if (event->normal.y < 0.0f) {
					if (koopa->GetCurrentHitPoints() > 0) {
						velocity.y = -deflectSpeed;
					}
					koopa->TakeDamage();
					koopa->SetNormal(D3DXVECTOR3(-this->normal.x, 0, 0));
				}
				else if (event->normal.x != 0.0f) {
					//is spinning or walking
					if (koopa->GetCurrentHitPoints() == 1 || koopa->GetCurrentHitPoints() == 3) {
						//mario is in racoon form and spins his tail
						if (Device::IsKeyDown(DIK_J) && velocity.x == 0.0f) {
							//stand still or die
							if (hitPoints == 4) {
								koopa->TakeDamage();
								koopa->SetNormal(D3DXVECTOR3(-this->normal.x, 0, 0));
							}
							else {
								TakeDamage();
								if (hitPoints > 0) {
									velocity.y = -deflectSpeed;
								}
								else {
									velocity.y -= dieflectSpeed;
								}
							}
						}
						else {
							TakeDamage();
							if (hitPoints > 0) {
								velocity.y = -deflectSpeed;
							}
							else {
								velocity.y -= dieflectSpeed;
							}
						}
					}
					//kick it, i dare you
					else if (koopa->GetCurrentHitPoints() == 2) {
						if (isHolding) {
							if (!heldEntity) {
								heldEntity = koopa;
							}
						}
						else {
							koopa->TakeDamage();
							koopa->SetNormal(D3DXVECTOR3(-this->normal.x, 0, 0));
						}
					}
				}
			}
		}
	}

	if (heldEntity) {
		if (heldEntity->GetCurrentHitPoints() == 3) {
			isHolding = false;
			heldEntity = nullptr;
			return;
		}

		if (isHolding) {
			int offset = 0;
			if (hitPoints == 1) {
				offset = 11;
			}
			else {
				offset = -4;
			}

			if (this->normal.x == 1) {
				heldEntity->SetPosition(D3DXVECTOR3(position.x + 12, position.y - offset, 0));
			}
			else {
				heldEntity->SetPosition(D3DXVECTOR3(position.x - 12, position.y - offset, 0));
			}
		}
		else {
			heldEntity->TakeDamage();
			heldEntity->SetNormal(D3DXVECTOR3(-this->normal.x, 0, 0));
			heldEntity = nullptr;
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
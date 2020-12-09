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
	hitPoints = 1;

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
	
	if (hitPoints > 0) {
		if (IsAttacking() && velocity.x == 0.0f) {
			bound.left = position.x - 8;
			bound.top = position.y;
			bound.right = bound.left + hitBox.GetWidth(1) * 2;
			bound.bottom = position.y + hitBox.GetHeight(1);
		}
		else {
			bound.left = position.x;
			bound.top = position.y;

			if (hitPoints == 1) {
				bound.right = position.x + hitBox.GetWidth(id);
				bound.bottom = position.y + hitBox.GetHeight(id);
			}
			else {
				bound.right = position.x + hitBox.GetWidth(1);
				bound.bottom = position.y + hitBox.GetHeight(1);
			}
		}
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

void Mario::ParseData(std::string dataPath, std::string texturePath, D3DCOLOR colorKey, std::vector<std::string> extraData) {
	std::ifstream readFile;
	readFile.open(dataPath, std::ios::in);

	if (!readFile.is_open()) {
		OutputDebugStringA("Failed to read data\n");
		return;
	}

	if (extraData.size() > 0) {
		this->extraData = extraData;
	}

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

void Mario::HandleMovement() {
	if (isOnGround) {
		gravity = 0.0025f;
	}
	
	//variable jump height by manupilating gravity
	//good enough	
	if (Device::IsKeyDown(DIK_K)) {
		if (gravity > MAX_GRAVITY) {
			gravity -= 0.0005f;
		}
		else if (gravity <= MAX_GRAVITY) {
			gravity = MAX_GRAVITY;
		}
	}
	else {
		if (gravity < 0.0025f) {
			gravity += 0.0005f;
		} 
	}

	//skid
	if (acceleration < ACCEL_THRESHOLD && velocity.x != 0.0f) {
		if (normal.x == -1) {
			if (Device::IsKeyDown(DIK_D)) {
				acceleration = 0.0499f;
			}
		}
		else if (normal.x == 1) {
			if (Device::IsKeyDown(DIK_A)) {
				acceleration = 0.0499f;
			}
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
		//slippery movement?
		//dont ask me
		//i lost track of all the plusses and minuses

		if (acceleration <= 0.5f) {
			velocity.x = 0.0f;
			acceleration = 0.5f;
		}

		if (acceleration > 0.5f) {
			acceleration -= 0.06f;
		}
	}
	
	if ((Device::IsKeyDown(DIK_A) || Device::IsKeyDown(DIK_D))) {
		//GOTTA GO FAAAST
		if (Device::IsKeyDown(DIK_J)) {
			if (acceleration < MAX_ACCEL) {
				acceleration += 0.03f;
			}
		}
		else {
			if (acceleration < MIN_ACCEL) {
				acceleration += 0.03f;
			}
			else if (acceleration > MIN_ACCEL) {
				acceleration -= 0.02f;
			}
		}
	}
}

void Mario::HandleStates(BYTE* states) {	
	//for picking up koopa shells
	if (Device::IsKeyDown(DIK_J)) {
		isHolding = true;
	}
	else {
		isHolding = false;
	}	

	HandleMovement();

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
		//DEBUG
		case DIK_1:
			hitPoints = 1;
			break;
		case DIK_2:
			hitPoints = 2;
			break;
		case DIK_3:
			hitPoints = 3;
			break;
		case DIK_4:
			hitPoints = 4;
			break;
		//DEBUG

		case DIK_A:
			normal.x = -1;
			break;
		case DIK_D:
			normal.x = 1;
			break;
		case DIK_J:
			if (hitPoints == 3 && !Device::IsKeyDown(DIK_S)) {
				if (fireballs.size() < 2) {
					fireballs.push_back(SpawnFireball());

					for (Fireball* fireball : fireballs) {
						SceneManager::GetInstance()->GetCurrentScene()->AddObjectToScene(fireball);
					}
				}				
			}

			if (hitPoints == 4) {
				if (!IsAttacking() && velocity.x == 0.0f) {
					StartAttackTimer();
				}
			}
			break;
		case DIK_K:
			//slow falling when in air
			if (!IsOnGround() && hitPoints == 4) {
				velocity.y *= 0.04f;
			}

			//unlimited jumping if Mario is Racoon
			if (hitPoints == 4) {
				if (acceleration >= ACCEL_THRESHOLD || IsFlying()) {
					//isOnGround false just to make the AnimatedSprite play the _TakeOffJump animation once
					if (!IsFlying() && isOnGround) {
						isOnGround = false;
						StartFlyTimer();
					}
					
					velocity.y = -jumpSpeed;
				}		
			}
			
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
	fireball->ParseData(extraData.at(0), extraData.at(1), colorKey);
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
		isOnGround = false;
		isHolding = false;
	}

	for (unsigned int i = 0; i < fireballs.size(); ++i) {
		if (fireballs.at(i)->GetCurrentHitPoints() == -1) {
			fireballs.erase(std::remove(fireballs.begin(), fireballs.end(), fireballs.at(i)), fireballs.end());
		}
	}

	if (attackStart != 0 && GetTickCount64() - attackStart > attackTime) {
		attackStart = 0;
	}

	//stops flying when time is up or mario get hit
	if (flyStart != 0 && (GetTickCount64() - flyStart > flyTime || hitPoints != 4)) {
		flyStart = 0;
	}

	GameObject::Update(delta);
	
	velocity.y += gravity * delta;
	
	//float just a little bit longer when flying
	if (acceleration >= ACCEL_THRESHOLD) {
		gravity = 0.0013f;
	}

	std::vector<LPCOLLISIONEVENT> collisionEvents, eventResults;
	collisionEvents.clear();

	if (hitPoints > 0) {
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

			//super leaf
			if (dynamic_cast<SuperLeaf*>(event->object)) {				
				if (hitPoints > 1) {
					hitPoints = 4;
				}
				else {
					hitPoints = 2;
				}

				SuperLeaf* leaf = static_cast<SuperLeaf*>(event->object);
				leaf->TakeDamage();
			}

			//super mushroom
			if (dynamic_cast<SuperMushroom*>(event->object)) {
				if (hitPoints <= 1) {
					hitPoints = 2;
				}

				SuperMushroom* mushroom = static_cast<SuperMushroom*>(event->object);
				mushroom->TakeDamage();
			}

			//1up mushroom
			if (dynamic_cast<GMushroom*>(event->object)) {
				GMushroom* mushroom = static_cast<GMushroom*>(event->object);
				mushroom->TakeDamage();
			}

			//venus fire's fireball
			if (dynamic_cast<Fireball*>(event->object) && event->object->GetObjectID() == 98) {
				TakeDamage();
			}

			//coin
			if (dynamic_cast<Coin*>(event->object)) {
				Coin* coin = static_cast<Coin*>(event->object);
				if (event->normal.x != 0.0f || event->normal.y != 0.0f) {
					coin->TakeDamage();
				}
			}

			//shiny brick
			if (dynamic_cast<ShinyBrick*>(event->object)) {
				ShinyBrick* shinyBrick = static_cast<ShinyBrick*>(event->object);
				if (event->normal.y > 0.0f) {
					shinyBrick->TakeDamage();
				}
			}

			//question block
			if (dynamic_cast<QuestionBlock*>(event->object)) {
				QuestionBlock* questionBlock = static_cast<QuestionBlock*>(event->object);
				if (event->normal.y > 0.0f) {
					questionBlock->TakeDamage();
				}
			}

			//switch block
			if (dynamic_cast<SwitchBlock*>(event->object)) {
				SwitchBlock* switchBlock = static_cast<SwitchBlock*>(event->object);
				if (event->normal.y < 0.0f) {
					switchBlock->TakeDamage();
					velocity.y = -deflectSpeed;
				}
			}

			//pirana plant or venus fire trap
			if (dynamic_cast<PiranaPlant*>(event->object)) {
				PiranaPlant* plant = static_cast<PiranaPlant*>(event->object);
				if (IsAttacking()) {
					plant->TakeDamage();
				}
				else {
					TakeDamage();
					if (hitPoints >= 0) {
						velocity.y = -deflectSpeed;
					}
				}
			}

			//goomba or paragoomba
			if (dynamic_cast<Goomba*>(event->object)) {					
				Goomba* goomba = static_cast<Goomba*>(event->object);
				if (event->normal.y < 0.0f) {
					if (goomba->GetCurrentHitPoints() > 0) {
						velocity.y = -deflectSpeed;
					}
					goomba->TakeDamage();
				}
				else if (event->normal.x != 0.0f || event->normal.y > 0.0f) {
					if (goomba->GetCurrentHitPoints() > 0) {
						if (IsAttacking()) {
							//stand still
							goomba->TakeDamage();
						}
						else {
							TakeDamage();
							if (hitPoints >= 0) {
								velocity.y = -deflectSpeed;
							}
						}
					}
				}
			}

			//koopa troopa or parakoopa
			if (dynamic_cast<KoopaTroopa*>(event->object)) {
				KoopaTroopa* koopa = static_cast<KoopaTroopa*>(event->object);
				//on koopa
				if (event->normal.y < 0.0f) {
					if (koopa->GetCurrentHitPoints() > 0) {
						velocity.y = -deflectSpeed;
					}

					if (koopa->GetCurrentHitPoints() != 1) {
						koopa->TakeDamage();
					}
					else if (koopa->GetCurrentHitPoints() == 1) {
						koopa->SetCurrenHitPoints(2);
						//dont know why koopa sunk a bit though when you jump on it the second time
						koopa->SetPosition(D3DXVECTOR3(koopa->GetPosition().x, koopa->GetPosition().y - 11, 0));
					}
				}
				else if (event->normal.x != 0.0f || event->normal.y > 0.0f) {
					//is spinning or walking
					if (koopa->GetCurrentHitPoints() == 1 || koopa->GetCurrentHitPoints() >= 3) {
						//mario is in racoon form and spins his tail
						if (IsAttacking()) {
							//stand still or die
							koopa->TakeDamage();
							koopa->SetScale(D3DXVECTOR2(1.0f, -1.0f));
						}
						else {
							TakeDamage();
							if (hitPoints >= 0) {
								velocity.y = -deflectSpeed;
							}
						}
					}
					//kick it, i dare you
					else if (koopa->GetCurrentHitPoints() == 2) {
						if (isHolding) {
							if (!heldEntity) {
								heldEntity = koopa;
								koopa->SetStatus(true); //is being held
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
		if (heldEntity->GetCurrentHitPoints() == 0 || heldEntity->GetCurrentHitPoints() == 3) {
			isHolding = false;
			
			if (heldEntity->GetCurrentHitPoints() == 3) {
				if (normal.x == 1) {
					heldEntity->SetPosition(D3DXVECTOR3(position.x + 17, position.y - 14, 0));
				}
				else {
					heldEntity->SetPosition(D3DXVECTOR3(position.x - 17, position.y - 14, 0));
				}
			}

			heldEntity->SetStatus(false);
			heldEntity = nullptr;
			return;
		}

		if (isHolding) {
			int offset = 0;
			if (hitPoints == 1) {
				offset = 11;
			}
			else {
				offset = 2;
			}

			if (normal.x == 1) {
				heldEntity->SetPosition(D3DXVECTOR3(position.x + 10, position.y - offset, 0));
			}
			else {
				heldEntity->SetPosition(D3DXVECTOR3(position.x - 10, position.y - offset, 0));
			}
		}
		else {
			heldEntity->TakeDamage();
			heldEntity->SetNormal(D3DXVECTOR3(-normal.x, 0, 0));
			heldEntity->SetStatus(false);
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

	fireballs.clear();
}
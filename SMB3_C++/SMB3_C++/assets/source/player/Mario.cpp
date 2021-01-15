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

		char debug[100];
		sprintf_s(debug, "[INSTANCE] Mario created, scene ID: %d\n", SceneManager::GetInstance()->GetCurrentScene()->GetSceneID());
		OutputDebugStringA(debug);
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

void Mario::ParseSettings(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 3) {
		return;
	}

	lives = std::stoi(tokens.at(0));
	coins = std::stoi(tokens.at(1));
	score = std::stoi(tokens.at(2));

	if (tokens.size() > 3) {
		for (unsigned int i = 0; i < tokens.size(); ++i) {
			bonusItems.push_back(static_cast<Entity::ObjectType>(std::stoi(tokens.at(i))));
		}
	}
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

		if (line == "[SETTINGS]") {
			section = DataSection::DATA_SECTION_SETTINGS;
		}

		switch (section) {
			case DataSection::DATA_SECTION_SPRITES:
				ParseSprites(line);
				break;
			case DataSection::DATA_SECTION_HITBOXES:
				ParseHitboxes(line);
				break;
			case DataSection::DATA_SECTION_SETTINGS:
				ParseSettings(line);
				break;
		}
	}

	marioFSM = new MarioStateMachine(marioInstance);

	readFile.close();
}

void Mario::GoRight() {
	scale = D3DXVECTOR2(-1.0f, 1.0f);
	velocity.x = runSpeed * acceleration;
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
					
					velocity.y = -jumpSpeed * 0.8f;
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
	if (!IsInvulnerable()) {
		if (hitPoints > 2) {
			hitPoints = 2;
		}
		else {
			--hitPoints;
		}

		StartInvulTimer();
	}
}

void Mario::HandleStageEnd() {
	if (triggeredStageEnd) {
		acceleration = MIN_ACCEL;
		isHolding = false;

		//score += SceneManager::GetInstance()->GetCurrentScene()->GetSceneTime() * 50;
	}
}

void Mario::HandleBonusItems() {	
	if (bonusItems.size() == 3) {
		int shroomCount = 0;
		int flowerCount = 0;
		int startCount = 0;

		for (unsigned int i = 0; i < bonusItems.size(); ++i) {
			switch (bonusItems.at(i)) {
				case Entity::ObjectType::OBJECT_TYPE_MUSHROOM:
					++shroomCount;
					break;
				case Entity::ObjectType::OBJECT_TYPE_FLOWER:
					++flowerCount;
					break;
				case Entity::ObjectType::OBJECT_TYPE_STAR:
					++startCount;
					break;
			}
		}

		if (shroomCount == 3) {
			lives += 2;
		}
		else if (flowerCount == 3) {
			lives += 3;
		}
		else if (startCount == 3) {
			lives += 5;
		}
		else {
			lives += 1;
		}

		bonusItems.clear();
	}
}

void Mario::Update(DWORD delta, std::vector<GameObject*>* objects) {
	if (SceneManager::GetInstance()->GetCurrentScene()->GetSceneID() == 10) {
		isInMap = true;
	}
	else {
		isInMap = false;
	}
	
	if (SceneManager::GetInstance()->GetCurrentScene()->GetSceneID() >= 11 &&
		SceneManager::GetInstance()->GetCurrentScene()->GetSceneTime() == 0) 
	{
		hitPoints = 0;
	}
	else if (SceneManager::GetInstance()->GetCurrentScene()->GetSceneID() == 10) {
		hitPoints = 1;
	}

	//stop moving if Mario dies
	if (hitPoints == 0) {
		velocity.x = 0;
		gravity = 0.0010f;
		isOnGround = false;
		isHolding = false;
	}

	HandleStageEnd();
	HandleBonusItems();

	//fire balls
	for (unsigned int i = 0; i < fireballs.size(); ++i) {
		if (fireballs.at(i)->GetCurrentHitPoints() == -1) {
			fireballs.erase(std::remove(fireballs.begin(), fireballs.end(), fireballs.at(i)), fireballs.end());
		}
	}

	//==TIMERS==
	if (attackStart != 0 && GetTickCount64() - attackStart > attackTime) {
		attackStart = 0;
	}

	//stops flying when time is up or mario get hit
	if (flyStart != 0 && (GetTickCount64() - flyStart > flyTime || hitPoints != 4)) {
		flyStart = 0;
	}

	if (invulStart != 0 && GetTickCount64() - invulStart > invulTime) {
		invulStart = 0;
	}
	//==TIMERS==

	if (triggeredStageEnd) {
		GoRight();
	}

	marioFSM->Update(delta);

	GameObject::Update(delta);	
	velocity.y += gravity * delta;
	
	//float just a little bit longer when flying
	if (acceleration >= ACCEL_THRESHOLD) {
		gravity = 0.0013f;
	}

	std::vector<LPCOLLISIONEVENT> collisionEvents, eventResults;

	if (hitPoints > 0) {
		CalcPotentialCollision(objects, collisionEvents);
	}

	if (collisionEvents.size() == 0) {
		position += distance;
	}
	else {
		D3DXVECTOR2 minTime;
		D3DXVECTOR2 offSet(0.4f, 0.4f);
		D3DXVECTOR3 normal;
		D3DXVECTOR3 relativeDistance;

		//deflect speed
		float ySpeed = 0.0f;

		FilterCollision(collisionEvents, eventResults, minTime, normal, relativeDistance);

		for (LPCOLLISIONEVENT result : eventResults) {
			LPCOLLISIONEVENT event = result;

			//enemies
			if (dynamic_cast<Entity*>(event->object) && 
				(event->object->GetObjectID() >= 1 && event->object->GetObjectID() <= 7) &&
				event->normal.x != 0.0f) 
			{
				touchedEntity = static_cast<Entity*>(event->object);
			}
			else {
				if (touchedEntity) {
					touchedEntity = nullptr;
				}
			}

			//isOnGround true when:
			//on tiles
			//on question block
			//on shiny bricks if their hp != 3
			if ((dynamic_cast<Tiles*>(event->object) || 
				dynamic_cast<QuestionBlock*>(event->object)) && 
				event->normal.y == -1.0f ||
				(dynamic_cast<ShinyBrick*>(event->object) && 
				dynamic_cast<ShinyBrick*>(event->object)->GetCurrentHitPoints() != 3))
			{
				isOnGround = true;
			}

			//platform... OF DEATH
			if (dynamic_cast<Tiles*>(event->object) && event->object->GetObjectID() == 666) {
				hitPoints = 0;
			}

			//bonus item
			if (dynamic_cast<BonusItem*>(event->object)) {
				BonusItem* bonusItem = static_cast<BonusItem*>(event->object);

				if (bonusItem->GetCurrentHitPoints() == 2) {
					bonusItems.push_back(bonusItem->GetItem());
				}

				bonusItem->TakeDamage();

				//phase through
				minTime.x = 1.0f;
				offSet.x = normal.x = relativeDistance.x = 0.0f;
				if (!isOnGround) {
					minTime.y = 1.0f;
					offSet.y = normal.y = relativeDistance.y = 0.0f;
				}

				triggeredStageEnd = true;
			}

			//super leaf
			if (dynamic_cast<SuperLeaf*>(event->object)) {
				if (hitPoints > 1) {
					hitPoints = 4;
				}
				else {
					hitPoints = 2;
					//to not fall out of the stage
					position.y -= GetBoxHeight();
				}

				SuperLeaf* leaf = static_cast<SuperLeaf*>(event->object);
				leaf->TakeDamage();

				minTime.x = 1.0f;
				offSet.x = normal.x = relativeDistance.x = 0.0f;
				if (!isOnGround) {
					minTime.y = 1.0f;
					offSet.y = normal.y = relativeDistance.y = 0.0f;
				}

				score += 1000;
			}

			//super mushroom
			if (dynamic_cast<SuperMushroom*>(event->object)) {
				if (hitPoints <= 1) {
					hitPoints = 2;
					position.y -= GetBoxHeight();
				}

				SuperMushroom* mushroom = static_cast<SuperMushroom*>(event->object);
				mushroom->TakeDamage();

				minTime.x = 1.0f;
				offSet.x = normal.x = relativeDistance.x = 0.0f;
				if (!isOnGround) {
					minTime.y = 1.0f;
					offSet.y = normal.y = relativeDistance.y = 0.0f;
				}

				score += 1000;
			}

			//1up mushroom
			if (dynamic_cast<GMushroom*>(event->object)) {
				GMushroom* mushroom = static_cast<GMushroom*>(event->object);
				mushroom->TakeDamage();

				minTime.x = 1.0f;
				offSet.x = normal.x = relativeDistance.x = 0.0f;
				if (!isOnGround) {
					minTime.y = 1.0f;
					offSet.y = normal.y = relativeDistance.y = 0.0f;
				}

				if (lives < 99) {
					++lives;
				}
			}

			//venus fire's fireball
			if (dynamic_cast<Fireball*>(event->object) && event->object->GetObjectID() == 98) {
				TakeDamage();

				minTime.x = 1.0f;
				offSet.x = normal.x = relativeDistance.x = 0.0f;
				if (!isOnGround) {
					minTime.y = 1.0f;
					offSet.y = normal.y = relativeDistance.y = 0.0f;
				}
			}

			//coin
			if (dynamic_cast<Coin*>(event->object)) {
				Coin* coin = static_cast<Coin*>(event->object);
				if (coin->GetCurrentHitPoints() == 1) {
					if (event->normal.x != 0.0f || event->normal.y != 0.0f) {
						coin->TakeDamage();

						++coins;
						score += 50;
					}
				}
				else if (coin->GetCurrentHitPoints() == 3) {
					if (event->normal.y > 0.0f) {
						coin->SetCurrenHitPoints(-1);
					}
				}
				
				if (coin->GetCurrentHitPoints() != 3) {
					minTime.x = 1.0f;
					offSet.x = normal.x = relativeDistance.x = 0.0f;
					if (!isOnGround) {
						minTime.y = 1.0f;
						offSet.y = normal.y = relativeDistance.y = 0.0f;
					}
				}
			}

			//shiny brick
			if (dynamic_cast<ShinyBrick*>(event->object)) {
				ShinyBrick* shinyBrick = static_cast<ShinyBrick*>(event->object);
				if (shinyBrick->GetCurrentHitPoints() == 2) {
					if (IsAttacking() || event->normal.y > 0.0f) {
						if (shinyBrick->GetExtraDataSize() == 0) {
							if (hitPoints > 1) {
								shinyBrick->SetCurrenHitPoints(-1);
							}
						}
						else {
							shinyBrick->TakeDamage();
						}
					}
				}
				else if (shinyBrick->GetCurrentHitPoints() == 3) {					
					minTime.x = 1.0f;
					offSet.x = normal.x = relativeDistance.x = 0.0f;
					if (!isOnGround) {
						minTime.y = 1.0f;
						offSet.y = normal.y = relativeDistance.y = 0.0f;
					}

					shinyBrick->SetCurrenHitPoints(-1);

					++coins;
					score += 50;
				}
			}

			//question block
			if (dynamic_cast<QuestionBlock*>(event->object)) {
				QuestionBlock* questionBlock = static_cast<QuestionBlock*>(event->object);
				if (IsAttacking() || event->normal.y > 0.0f) {
					questionBlock->TakeDamage();
				}
			}

			//switch block
			if (dynamic_cast<SwitchBlock*>(event->object)) {
				SwitchBlock* switchBlock = static_cast<SwitchBlock*>(event->object);
				if (event->normal.y < 0.0f) {
					switchBlock->TakeDamage();
					ySpeed = -deflectSpeed;
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
						ySpeed = -deflectSpeed;
					}
				}
			}

			//goomba or paragoomba
			if (dynamic_cast<Goomba*>(event->object)) {					
				Goomba* goomba = static_cast<Goomba*>(event->object);
				if (event->normal.y < 0.0f) {
					if (goomba->GetCurrentHitPoints() > 0) {
						ySpeed = -deflectSpeed;
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
								ySpeed = -deflectSpeed;
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
						ySpeed = -deflectSpeed;
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
								ySpeed = -deflectSpeed;
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

		if (normal.y != 0.0f) {
			velocity.y = ySpeed;
		}

		position.x += distance.x * minTime.x + normal.x * offSet.x;
		position.y += distance.y * minTime.y + normal.y * offSet.y;
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
		triggeredStageEnd = false;

		for (unsigned int i = 0; i < fireballs.size(); ++i) {
			if (fireballs.at(i)->GetCurrentHitPoints() == -1) {
				fireballs.erase(std::remove(fireballs.begin(), fireballs.end(), fireballs.at(i)), fireballs.end());
			}
		}
		fireballs.clear();

		if (texturePath) {
			delete texturePath;
			texturePath = nullptr;
		}

		if (texture) {
			texture->Release();
			texture = nullptr;
		}

		if (marioFSM) {
			marioFSM->Release();
		}

		//delete marioInstance;
		marioInstance = nullptr;
	}
}
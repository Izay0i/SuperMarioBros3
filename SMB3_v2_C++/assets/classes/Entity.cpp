#include "spatial/Grid.h"
#include "Entity.h"

void Entity::_ParseHitboxes(std::string line) {
	std::vector<std::string> tokens = GlobalUtil::SplitStr(line);

	if (tokens.size() < 4) {
		return;
	}

	RECTF hitbox;
	hitbox.left = std::stof(tokens.at(0));
	hitbox.top = std::stof(tokens.at(1));
	hitbox.right = std::stof(tokens.at(2));
	hitbox.bottom = std::stof(tokens.at(3));

	_hitbox.AddHitbox(hitbox);
}

bool Entity::CompareRenderPriority(Entity*& a, Entity*& b) {
	return a->_renderPriority < b->_renderPriority;
}

Entity::Entity() {
	_health = 1;
	_renderPriority = std::numeric_limits<unsigned int>::max();
	_score = 100;
	_scoreMultiplier = 1;
	_removeTime = 2000;

	ownerCell = nullptr;
}

Entity::~Entity() {}

void Entity::SetHealth(int health) {
	if (health > -2) {
		_health = health;
	}
}

int Entity::GetHealth() const {
	return _health;
}

void Entity::ParseData(
	std::string dataPath, 
	const LPDIRECT3DTEXTURE9& texture,  
	std::vector<std::string> extraData) 
{
	std::ifstream readFile;
	readFile.open(dataPath, std::ios::in);

	if (!readFile.is_open()) {
		OutputDebugStringA("[ENTITY] Failed to read data\n");
		return;
	}

	if (!extraData.empty()) {
		_extraData = extraData;
	}

	_DataFileSection dataFileSection = _DataFileSection::DATAFILE_SECTION_UNKNOWN;
	char str[GlobalUtil::MAX_FILE_LINE];
	while (readFile.getline(str, GlobalUtil::MAX_FILE_LINE)) {
		std::string line(str);

		if (line.empty() || line.front() == '#') {
			continue;
		}

		if (line == "[/]") {
			dataFileSection = _DataFileSection::DATAFILE_SECTION_UNKNOWN;
			continue;
		}

		if (line == "[SPRITES]") {
			dataFileSection = _DataFileSection::DATAFILE_SECTION_SPRITES;
			continue;
		}

		if (line == "[HITBOXES]") {
			dataFileSection = _DataFileSection::DATAFILE_SECTION_HITBOXES;
			continue;
		}

		switch (dataFileSection) {
			case _DataFileSection::DATAFILE_SECTION_SPRITES:
				_ParseSprites(line);
				break;
			case _DataFileSection::DATAFILE_SECTION_HITBOXES:
				_ParseHitboxes(line);
				break;
		}
	}

	readFile.close();
}

void Entity::TakeDamage() {
	if (_health > -1) {
		--_health;
	}
}

void Entity::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (!_isActive && _objectType >= GameObjectType::GAMEOBJECT_TYPE_GOOMBA) {
		return;
	}
	
	GameObject::Update(deltaTime);
	_velocity.y += _gravity * _deltaTime;

	std::vector<LPCOLLISIONEVENT> collisionEvents, eventResults;
	if (_health > 0) {
		if (grid != nullptr) {
			//Edge case: what if the entity's bounding box is larger than the grid cell size? i.e: tile->_hitbox > grid->_cellsize
			//Use another collection (collidableTiles) and calculate the collisions
			//Since tiles themselves are static, as in, they don't update and render every tick, performance wise it's negligible
			CalcPotentialCollision(collidableTiles, collisionEvents);
			
			//Check collisions from the residing cell
			CalcPotentialCollision(&ownerCell->entities, collisionEvents);

			//Check collisions from neighboring cells
			//Only need half of the neighboring cells to avoid double checking
			// cell	cell
			// cell	entity
			// cell
			//Left
			if (ownerCell->indexX > 0) {
				CalcPotentialCollision(
					&grid->GetCell(ownerCell->indexX - 1, ownerCell->indexY)->entities, 
					collisionEvents
				);
				//Top left
				if (ownerCell->indexY > 0) {
					CalcPotentialCollision(
						&grid->GetCell(ownerCell->indexX - 1, ownerCell->indexY - 1)->entities,
						collisionEvents
					);
				}
				//Bottom left
				if (ownerCell->indexY < grid->_yCells - 1) {
					CalcPotentialCollision(
						&grid->GetCell(ownerCell->indexX - 1, ownerCell->indexY + 1)->entities, 
						collisionEvents
					);
				}
			}
			//Top
			if (ownerCell->indexY > 0) {
				CalcPotentialCollision(
					&grid->GetCell(ownerCell->indexX, ownerCell->indexY - 1)->entities, 
					collisionEvents
				);				
			}
		}
		else {
			CalcPotentialCollision(collidableTiles, collisionEvents);
			CalcPotentialCollision(collidableEntities, collisionEvents);
		}
	}

	if (collisionEvents.empty()) {
		_position += _distance;
	}
	else {
		D3DXVECTOR2 minTime;
		D3DXVECTOR2 offset(0.4f, 0.4f);
		D3DXVECTOR2 normal;
		D3DXVECTOR2 relativeDistance;

		FilterCollision(collisionEvents, eventResults, minTime, normal, relativeDistance);
		for (LPCOLLISIONEVENT result : eventResults) {
			HandleCollisionResult(result, minTime, offset, normal, relativeDistance);
		}

		if (normal.x != 0.0f) {
			_velocity.x = 0.0f;
		}

		if (normal.y != 0.0f) {
			_velocity.y = 0.0f;
		}

		_position.x += _distance.x * minTime.x + normal.x * offset.x;
		_position.y += _distance.y * minTime.y + normal.y * offset.y;
	}

	for (LPCOLLISIONEVENT event : collisionEvents) {
		delete event;
	}
}

CollisionEvent* Entity::SweptAABBEx(Entity*& entity) {
	RECTF movingEntity;
	RECTF staticEntity;
	D3DXVECTOR2 normal;
	float time;

	staticEntity = entity->GetBoundingBox();
	D3DXVECTOR2 staticVeloctity = entity->GetVelocity();
	D3DXVECTOR2 staticDistance = staticVeloctity * static_cast<float>(_deltaTime);
	D3DXVECTOR2 relativeDistance = _distance - staticDistance;

	movingEntity = this->GetBoundingBox();
	SweptAABB(movingEntity, staticEntity, relativeDistance, normal, time);

	if (entity->GetObjectType() == GameObject::GameObjectType::GAMEOBJECT_TYPE_ONEWAYPLATFORM) {
		normal.x = 0.0f;
	}

	return new CollisionEvent(entity, normal, relativeDistance, time);
}

void Entity::CalcPotentialCollision(std::vector<Entity*>* collidableEntities, std::vector<LPCOLLISIONEVENT>& collisionEvents) {
	if (collidableEntities == nullptr) {
		return;
	}
	
	for (unsigned int i = 0; i < collidableEntities->size(); ++i) {
		LPCOLLISIONEVENT event = SweptAABBEx(collidableEntities->at(i));
		if (event->time >= 0.0f && event->time <= 1.0f) {
			collisionEvents.emplace_back(event);
		}
		else {
			delete event;
			event = nullptr;
		}
	}

	std::sort(collisionEvents.begin(), collisionEvents.end(), CollisionEvent::CompareCollisionEvent);
}
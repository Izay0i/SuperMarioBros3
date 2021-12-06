#include "SceneManager.h"
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
	return a->_renderPriority > b->_renderPriority;
}

Entity::Entity() {
	_health = 1;
	_renderPriority = std::numeric_limits<unsigned int>::max();
	_removeTime = 800;

	ownerCell = nullptr;
}

Entity::~Entity() {}

bool Entity::IsRemoved() const {
	return _removeStart != 0;
}

void Entity::StartRemoveTimer() {
	_removeStart = static_cast<DWORD>(GetTickCount64());
}

std::vector<std::string> Entity::GetExtraData() const {
	return _extraData;
}

void Entity::SetGravity(float gravity) {
	_gravity = gravity;
}

float Entity::GetGravity() const {
	return _gravity;
}

void Entity::SetHealth(int health) {
	_health = health;
}

int Entity::GetHealth() const {
	return _health;
}

void Entity::ParseData(
	std::string dataPath, 
	Texture*& texture,  
	std::vector<std::string> extraData) 
{
	std::ifstream readFile;
	readFile.open(dataPath, std::ios::in);

	if (!readFile.is_open()) {
		char debug[100];
		sprintf_s(debug, "[ENTITY] Failed to read data for object with ID: %d\n", _objectType);
		OutputDebugStringA(debug);
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
	if (_health > 0) {
		--_health;
		tookDamage = true;
	}
}

void Entity::HandleOverlap(Entity* entity) {}

void Entity::Update(
	DWORD deltaTime, 
	std::vector<Entity*>* collidableEntities, 
	std::vector<Entity*>* collidableTiles, 
	Grid* grid) 
{
	if (_position.y > SceneManager::GetInstance()->GetCurrentScene()->GetSceneHeight()) {
		_health = 0;
	}

	if (IsRemoved() && GetTickCount64() - _removeStart > _removeTime) {
		_health = -1;
		_removeStart = 0;
	}

	if (!_isActive) {
		return;
	}
	
	GameObject::Update(deltaTime);
	_velocity.y += _gravity * _deltaTime;

	std::vector<LPCOLLISIONEVENT> collisionEvents, eventResults;
	if (_health > 0) {
		if (collidableEntities != nullptr) {
			for (unsigned int i = 0; i < collidableEntities->size(); ++i) {
				Entity* entity = collidableEntities->at(i);
				if (entity != this && IsOverlapped(entity)) {
					HandleOverlap(entity);
				}
			}
		}

		//Edge case: what if the entity's bounding box is larger than the grid cell size? i.e: tile->_hitbox > grid->_cellsize
		//Use another collection (collidableTiles) and calculate the collisions
		//Since tiles themselves are static, as in, they don't update and render every tick, performance wise it's negligible
		CalcPotentialCollision(collidableTiles, collisionEvents);

		if (grid != nullptr) {	
			//Check collisions from the residing cell
			CalcPotentialCollision(&ownerCell->entities, collisionEvents);

			//Check collisions from neighboring cells
			//Only need half of the neighboring cells to avoid double checking
			//		cell
			// cell	entity cell
			//		cell
			//Left
			if (ownerCell->indexX > 0) {
				CalcPotentialCollision(
					&grid->GetCell(ownerCell->indexX - 1, ownerCell->indexY)->entities, 
					collisionEvents
				);
			}
			//Right
			if (ownerCell->indexX < grid->_xCells - 1) {
				CalcPotentialCollision(
					&grid->GetCell(ownerCell->indexX + 1, ownerCell->indexY)->entities,
					collisionEvents
				);
			}
			//Top
			if (ownerCell->indexY > 0) {
				CalcPotentialCollision(
					&grid->GetCell(ownerCell->indexX, ownerCell->indexY - 1)->entities,
					collisionEvents
				);
			}
			//Bottom
			if (ownerCell->indexY < grid->_yCells - 1) {
				CalcPotentialCollision(
					&grid->GetCell(ownerCell->indexX, ownerCell->indexY + 1)->entities,
					collisionEvents
				);
			}
		}
		else {
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

		if (normal.x != 0.0f) {
			_velocity.x = 0.0f;
		}

		if (normal.y != 0.0f) {
			_velocity.y = 0.0f;
		}

		for (LPCOLLISIONEVENT result : eventResults) {
			HandleCollisionResult(result, minTime, offset, normal, relativeDistance);
		}

		_position.x += _distance.x * minTime.x + normal.x * offset.x;
		_position.y += _distance.y * minTime.y + (normal.y == -1.0f ? (normal.y * offset.y) : 0.0f);
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

	movingEntity = this->GetBoundingBox();

	staticEntity = entity->GetBoundingBox();
	D3DXVECTOR2 staticDistance = entity->GetDistance();

	//_distance is already calculated
	//See GameObject::Update
	D3DXVECTOR2 relativeDistance = this->_distance - staticDistance;

	SweptAABB(movingEntity, staticEntity, relativeDistance, normal, time);

	if (entity->GetObjectType() == GameObjectType::GAMEOBJECT_TYPE_ONEWAYPLATFORM) {
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
		}
	}

	std::sort(collisionEvents.begin(), collisionEvents.end(), CollisionEvent::CompareCollisionEvent);
}
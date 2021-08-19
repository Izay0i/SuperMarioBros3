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

bool Entity::CompareRenderPriority(const Entity*& a, const Entity*& b) {
	return a->_renderPriority < b->_renderPriority;
}

Entity::Entity() {
	_health = 1;
	_renderPriority = std::numeric_limits<unsigned int>::max();
	_score = 100;
	_scoreMultiplier = 1;

	cellIndex = std::numeric_limits<unsigned int>::max();
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

AnimatedSprite Entity::GetAnimatedSprite() const {
	return _animatedSprite;
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

void Entity::Update(DWORD deltaTime, std::vector<Entity*>* collidableEntities) {
	GameObject::Update(deltaTime);

	std::vector<LPCOLLISIONEVENT> collisionEvents, eventResults;
	if (_health > 0) {
		CalcPotentialCollision(collidableEntities, collisionEvents);
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
			HandleCollisionEventResults(result, minTime, offset, normal, relativeDistance);
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
	for (unsigned int i = 0; i < collidableEntities->size(); ++i) {
		LPCOLLISIONEVENT event = SweptAABBEx(collidableEntities->at(i));
		if (event->time >= 0.0f && event->time <= 1.0f) {
			collisionEvents.push_back(event);
		}
		else {
			delete event;
			event = nullptr;
		}
	}

	std::sort(collisionEvents.begin(), collisionEvents.end(), CollisionEvent::CompareCollisionEvent);
}
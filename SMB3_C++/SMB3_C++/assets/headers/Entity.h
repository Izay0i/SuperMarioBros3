#pragma once

#include "GameObject.h"
#include "AnimatedSprite.h"
#include "HitBox.h"

class AnimatedSprite;
class HitBox;

class Entity : public GameObject {
protected:
	enum class DataSection {
		DATA_SECTION_UNKNOWN,
		DATA_SECTION_SPRITES,
		DATA_SECTION_HITBOXES
	};

	float moveSpeed;

	AnimatedSprite* sprite;
	HitBox hitBox;

	virtual void CheckCollision(Entity*, Entity*) = 0;

	virtual	void SetPosition(D3DXVECTOR3) = 0;
	virtual	D3DXVECTOR3 GetPosition() = 0;

	virtual	void SetRotation(D3DXVECTOR2) = 0;
	virtual	D3DXVECTOR2 GetRotation() = 0;

	virtual	void SetTranslation(D3DXVECTOR2) = 0;
	virtual	D3DXVECTOR2 GetTranslation() = 0;

	virtual	void SetScale(D3DXVECTOR2) = 0;
	virtual	D3DXVECTOR2 GetScale() = 0;

public:
	virtual ~Entity() {}

	virtual void Update(DWORD) = 0;
	virtual void Render() = 0;

	virtual void Release() = 0;
};
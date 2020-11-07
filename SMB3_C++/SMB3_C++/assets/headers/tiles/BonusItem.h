#pragma once

#include <fstream>

#include "../Entity.h"

class Entity;

class BonusItem : public Entity {
private:
	enum class ItemState {
		ROTATE,
		PICKEDUP
	};

	const static int MAX_FILE_LINE = 1024;

	LPDIRECT3DDEVICE9 directDevice;
	LPD3DXSPRITE spriteHandler;

	void CheckCollision(Entity*, Entity*) override {}

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

public:
	BonusItem();

	void ParseData(std::string);

	void SetPosition(D3DXVECTOR3) override {}
	D3DXVECTOR3 GetPosition() override { return position; }

	void SetRotation(D3DXVECTOR2) override {}
	D3DXVECTOR2 GetRotation() override { return rotation; }

	void SetTranslation(D3DXVECTOR2) override {}
	D3DXVECTOR2 GetTranslation() override { return translation; }

	void SetScale(D3DXVECTOR2) override {}
	D3DXVECTOR2 GetScale() override { return scale; }

	void Update(DWORD) override;
	void Render() override;

	void Release() override;
};
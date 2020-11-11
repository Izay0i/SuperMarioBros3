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

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	void LoadTexture();

	void CheckCollision(Entity*, Entity*) override {}

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

public:
	BonusItem();

	void ParseData(std::string, std::string, D3DCOLOR) override;

	RECTF GetBoundingBox(int = 0) const override { return hitBox.GetBoundingBox(0); }

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
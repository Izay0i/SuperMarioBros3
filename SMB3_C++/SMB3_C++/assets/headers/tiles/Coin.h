#pragma once

#include <fstream>

#include "../Entity.h"

class Entity;

class Coin : public Entity {
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
	Coin();

	void ParseData(std::string, std::string, D3DCOLOR) override;

	void Update(DWORD) override;
	void Render() override;

	void Release() override;
};
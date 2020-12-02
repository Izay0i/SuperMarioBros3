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

	DWORD removeTime = 100;

	void LoadTexture();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

	void HandleStates();

public:
	Coin();

	void ParseData(std::string, std::string, D3DCOLOR) override;

	RECTF GetBoundingBox(int id = 0) const override {
		RECTF bound;
		
		if (hitPoints > 0) {
			bound.left = position.x + hitBox.GetWidth(id);
			bound.top = position.y + hitBox.GetHeight(id);
			bound.right = position.x + hitBox.GetWidth(id);
			bound.bottom = position.y + hitBox.GetHeight(id);
		}

		return bound;
	}

	void TakeDamage() override;

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
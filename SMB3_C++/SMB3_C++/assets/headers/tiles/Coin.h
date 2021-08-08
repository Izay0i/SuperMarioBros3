#pragma once

//didn't think the coin would move so i put it in the tile folder
//this is why you plan ahead

#include <fstream>

#include "../Entity.h"

class Entity;

class Coin : public Entity {
private:
	enum class ItemState {
		ROTATE,
		POPPEDOUTOFQBLOCK,
		SWITCHEDTOBRICK
	};

	const static int MAX_FILE_LINE = 1024;

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	ItemState currentState;

	DWORD removeTime = 50;

	float gravity = 0.0005f;

	void LoadTexture();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

	void HandleStates();

public:
	Coin();

	void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>()) override;

	RECTF GetBoundingBox(int id = 0) const override {
		RECTF bound;
		
		if (hitPoints > 0) {
			bound.left = position.x;
			bound.top = position.y;
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
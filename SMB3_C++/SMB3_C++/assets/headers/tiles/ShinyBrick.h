#pragma once

#include <fstream>

#include "../Entity.h"

#include "../npc/1UPMushroom.h"
#include "../tiles/SwitchBlock.h"
#include "../tiles/Coin.h"

class Entity;

class ShinyBrick : public Entity {
private:
	enum class BlockState {
		ROTATE,
		PUSHED,
		SWITCHED
	};

	const static int MAX_FILE_LINE = 1024;
	const float MAX_Y_OFFSET = 0.4f; //how far up the object can go

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	BlockState currentState;

	unsigned int amount;

	float jumpSpeed = 0.4f;
	float gravity = 0.001f;

	D3DXVECTOR3 originalPos;

	void LoadTexture();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

	void HandleStates();

public:
	ShinyBrick();

	void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>()) override;

	RECTF GetBoundingBox(int id = 0) const override {
		RECTF bound;
		bound.left = position.x + 2;
		bound.top = position.y + 1;
		bound.right = position.x + hitBox.GetWidth(id) - 3;
		bound.bottom = position.y + hitBox.GetHeight(id) - 5;

		return bound;
	}

	void SetPosition(D3DXVECTOR3 pos) override { position = pos; originalPos = position; }

	Entity* SpawnItem();
	
	//7 or 11 coins
	//std::vector<Coin*> coins;

	void TakeDamage() override;

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
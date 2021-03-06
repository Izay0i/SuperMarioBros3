#pragma once

#include <fstream>

#include "../Entity.h"

#include "../tiles/Coin.h"
#include "../npc/SuperLeaf.h"
#include "../npc/SuperMushroom.h"

class Entity;

class Coin;
class SuperLeaf;
class SuperMushroom;

class QuestionBlock : public Entity {
private:
	enum class BlockState {
		ROTATE,
		PUSHED
	};

	const static int MAX_FILE_LINE = 1024;
	const float MAX_Y_OFFSET = 0.4f; //how far up the object can go

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;
	
	BlockState currentState;

	float jumpSpeed = 0.4f;
	float gravity = 0.001f;

	D3DXVECTOR3 originalPos;

	void LoadTexture();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

	void HandleStates();

public:
	QuestionBlock();

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

	Entity* SpawnItem(int);

	int GetScore() override { return score; }

	void TakeDamage() override;

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
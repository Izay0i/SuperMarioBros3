#pragma once

#include <fstream>

#include "../Entity.h"

class Entity;

class QuestionBlock : public Entity {
private:
	enum class BlockState {
		ROTATE,
		PUSHED
	};

	const static int MAX_FILE_LINE = 1024;

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

	void ParseData(std::string, std::string, D3DCOLOR) override;

	RECTF GetBoundingBox(int id = 0) const override {
		RECTF bound;
		bound.left = position.x + 2;
		bound.top = position.y;
		bound.right = position.x + hitBox.GetWidth(id) - 3;
		bound.bottom = position.y + hitBox.GetHeight(id) - 5;

		return bound;
	}

	void SetPosition(D3DXVECTOR3 pos) override { position = pos; originalPos = position; }

	void TakeDamage() override;

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
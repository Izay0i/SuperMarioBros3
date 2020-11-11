#pragma once

#include <fstream>

#include "../Entity.h"

class Entity;

class QuestionBlock : public Entity {
private:
	enum class ItemState {
		ROTATE,
		PUSHED
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
	QuestionBlock();

	void ParseData(std::string, std::string, D3DCOLOR) override;

	RECTF GetBoundingBox(int id = 0) const override {
		RECTF bound;
		bound.left = position.x;
		bound.top = position.y;
		bound.right = position.x + hitBox.GetWidth(0);
		bound.bottom = position.y + hitBox.GetHeight(0);

		return bound;
	}

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
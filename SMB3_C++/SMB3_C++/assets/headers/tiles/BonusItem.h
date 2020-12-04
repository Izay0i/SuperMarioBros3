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

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

public:
	BonusItem();

	void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>()) override;

	RECTF GetBoundingBox(int id = 0) const override {
		RECTF bound;
		bound.left = position.x + 1;
		bound.top = position.y;
		bound.right = position.x + hitBox.GetWidth(id);
		bound.bottom = position.y + hitBox.GetHeight(id);

		return bound;
	}

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
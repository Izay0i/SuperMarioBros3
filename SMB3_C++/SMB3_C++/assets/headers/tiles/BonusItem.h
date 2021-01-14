#pragma once

#include <fstream>
#include <random>

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

	ItemState currentState;
	Entity::ObjectType currentItem;

	DWORD removeTime = 3000;

	void LoadTexture();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

	void HandleStates();

public:
	BonusItem();

	void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>()) override;

	Entity::ObjectType GetItem() const { return currentItem; }

	RECTF GetBoundingBox(int = 0) const override;

	void TakeDamage() override;

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
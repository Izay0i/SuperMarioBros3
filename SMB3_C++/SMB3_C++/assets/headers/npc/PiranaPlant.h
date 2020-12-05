#pragma once

#include <fstream>

#include "../Entity.h"

class Entity;

class PiranaPlant : public Entity {
protected:
	enum class PlantState {
		BITING,
		DIE
	};

	const static int MAX_FILE_LINE = 1024;
	const float MAX_Y_OFFSET = 16.0f; //how far up should the plant go
	const float MIN_Y_OFFSET = 16.0f; //how far down should the plant go

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	PlantState currentState;

	DWORD removeTime = 100;
	
	virtual void LoadTexture();

	virtual void ParseSprites(std::string);
	virtual void ParseHitboxes(std::string);

	virtual void HandleStates();

public:
	PiranaPlant();

	virtual RECTF GetBoundingBox(int id = 0) const override;

	virtual void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>()) override;

	virtual void TakeDamage() override;

	virtual void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	virtual void Render() override;

	virtual void Release() override;
};
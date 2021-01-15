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
	const float MAX_Y_OFFSET = 48.0f; //how far up should the plant go
	const float MIN_Y_OFFSET = 32.0f; //how far down should the plant go

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	bool playerInRange;

	PlantState currentState;

	DWORD removeTime = 100;
	
	DWORD retractStart;
	DWORD retractTime = 2000;

	D3DXVECTOR3 originalPos;

	virtual void LoadTexture();

	virtual void ParseSprites(std::string);
	virtual void ParseHitboxes(std::string);

	virtual void HandleStates();

public:
	PiranaPlant();

	virtual RECTF GetBoundingBox(int = 0) const override;

	virtual void PlayerIsInRange(bool b) { playerInRange = b; }

	virtual void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>()) override;

	virtual void StartRetractTimer() { retractStart = static_cast<DWORD>(GetTickCount64()); }
	virtual bool IsRetracting() { return retractStart != 0; }

	virtual void SetPosition(D3DXVECTOR3 pos) override { position = pos; originalPos = position; }

	virtual void TakeDamage() override;

	virtual void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	virtual void Render() override;

	virtual void Release() override;
};
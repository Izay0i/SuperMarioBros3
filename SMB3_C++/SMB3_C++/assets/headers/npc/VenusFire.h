#pragma once

#include <fstream>

#include "../Entity.h"
#include "../TileList.h"
#include "PiranaPlant.h"
#include "Fireball.h"
#include "../SceneManager.h"

class Entity;

class VenusFire : public PiranaPlant {
private:
	float MAX_Y_OFFSET = 48.0f;
	DWORD retractTime = 5000;

public:
	VenusFire();

	Fireball* SpawnFireball();

	void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>()) override;

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
#pragma once

#include <fstream>

#include "../Entity.h"
#include "../TileList.h"
#include "PiranaPlant.h"

class Entity;

class VenusFire : public PiranaPlant {
public:
	VenusFire();

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
#pragma once

#include <fstream>

#include "../Entity.h"

class Entity;

class Curtain : public Entity {
private:
	const static int MAX_FILE_LINE = 1024;

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	DWORD delayStart;
	DWORD delayTime = 1000;

	void LoadTexture();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

public:
	Curtain();

	void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>()) override;

	RECTF GetBoundingBox(int = 0) const;

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
#pragma once

#include <fstream>

#include "../Entity.h"
#include "../SceneManager.h"

class Entity;

class Boomerang : public Entity {
private:
	const float MAX_DISTANCE = 128.0f;

	const static int MAX_FILE_LINE = 1024;

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	float moveSpeed = 0.1f;
	
	DWORD removeTime = 2000;

	D3DXVECTOR3 originalPos;

	void LoadTexture();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

public:
	Boomerang();

	void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>()) override;

	RECTF GetBoundingBox(int = 0) const;

	void SetPosition(D3DXVECTOR3 pos) override { position = pos; originalPos = position; }

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
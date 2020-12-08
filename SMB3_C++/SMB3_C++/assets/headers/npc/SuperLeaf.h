#pragma once

#include "../Entity.h"
#include "../TileList.h"

class Entity;

class SuperLeaf : public Entity {
private:
	const static int MAX_FILE_LINE = 1024;

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;
	
	float runSpeed = 0.02f;
	float jumpSpeed = 0.2f;
	float gravity = 0.002f;

	DWORD removeTime = 300;

	void LoadTexture();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

	void HandleStates();

public:
	SuperLeaf();

	RECTF GetBoundingBox(int = 0) const override;

	void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>()) override;
	
	void TakeDamage() override;

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
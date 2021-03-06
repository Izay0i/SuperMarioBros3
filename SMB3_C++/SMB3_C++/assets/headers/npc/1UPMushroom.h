#pragma once

#include "../Entity.h"
#include "../TileList.h"

class Entity;

class GMushroom : public Entity {
private:
	const static int MAX_FILE_LINE = 1024;

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	float runSpeed = 0.002f;
	float jumpSpeed = 0.2f;
	float gravity = 0.002f;

	DWORD removeTime = 50;

	DWORD emergeStart;
	DWORD emergeTime = 700;

	void LoadTexture();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

	void HandleStates();

public:
	GMushroom();

	RECTF GetBoundingBox(int = 0) const override;

	void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>()) override;

	void StartEmergeTimer() { emergeStart = static_cast<DWORD>(GetTickCount64()); }

	void TakeDamage() override;

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
#pragma once

#include <fstream>

#include "../Entity.h"

class Entity;

class MovingPlatform : public Entity {
private:
	enum class PlatformState {
		FALLING,
		MOVING
	};

	const static int MAX_FILE_LINE = 1024;

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	PlatformState currentState;

	float moveSpeed = 0.03f;
	float gravity = 0.0002f;

	void LoadTexture();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

	void HandleStates();

public:
	MovingPlatform();

	void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>()) override;

	RECTF GetBoundingBox(int = 0) const;

	void TakeDamage() override;

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
#pragma once

#include "../Entity.h"
#include "../TileList.h"

class Entity;

class Fireball : public Entity {
private:
	enum class BallState {
		BOUNCE,
		EXPLODE
	};

	const static int MAX_FILE_LINE = 1024;

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	BallState currentState;
	
	float travelSpeed = 0.0012f;
	float runSpeed = 0.2f;
	float jumpSpeed = 0.2f;
	float gravity = 0.002f;

	DWORD removeTime = 300;

	DWORD aliveStart;
	DWORD aliveTime = 10000;

	void LoadTexture();

	void ParseSprites(std::string);
	void ParseHitboxes(std::string);

	void HandleStates();

public:
	Fireball();

	RECTF GetBoundingBox(int = 0) const override;

	void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>()) override;

	void StartCountDownTimer() { aliveStart = static_cast<DWORD>(GetTickCount64()); }
	bool IsAlive() const { return aliveStart != 0; }

	void TakeDamage() override;

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
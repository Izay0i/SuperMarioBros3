#pragma once

#include <fstream>
#include <string>

#include <d3dx9.h>

#include "../Entity.h"
#include "../Util.h"

class HUD : public Entity {
private:
	static HUD* hudInstance;

	const static int MAX_FILE_LINE = 1024;

	static LPCWSTR texturePath;
	static LPDIRECT3DTEXTURE9 texture;
	static D3DCOLOR colorKey;

	const unsigned int LIVES_MAX_LENGTH = 2;
	const unsigned int COINS_MAX_LENGTH = 2;
	const unsigned int SCORE_MAX_LENGTH = 7;
	const unsigned int TIME_MAX_LENGTH = 3;

	//end me
	//Components
	//==Lives
	std::vector<std::string> livesArr;
	//==Coins==
	std::vector<std::string> coinsArr;
	//==Items==
	std::vector<std::string> itemsArr;
	//==Speed Gauge==
	std::vector<std::string> speedGaugeArr;
	//==Score==
	std::vector<std::string> scoreArr;
	//==Time==
	std::vector<std::string> timeArr;

	void LoadTexture();

	void ParseSprites(std::string);

	void ParseLives(unsigned int);
	void ParseCoins(unsigned int);
	void ParseItems(std::vector<Entity::ObjectType>);
	void ParseSpeedGauge(float, float, bool, bool);
	void ParseScore(unsigned int);
	void ParseTime(unsigned long);

	std::string DigitToString(std::string);

public:
	static HUD* GetInstance();
	
	void ParseData(std::string, std::string, D3DCOLOR, std::vector<std::string> = std::vector<std::string>()) override;

	RECTF GetBoundingBox(int id = 0) const override { return RECTF(); }

	//delta, lives, coins, items, speed gauge(current acceleration), max acceleration, score, timer, is flying, sprint key pressed
	void Update(DWORD, const unsigned int, const unsigned int, const std::vector<Entity::ObjectType>, const float, const float, const unsigned int, const unsigned long, bool, bool);

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override {}
	void Render() override;

	void Release();
};
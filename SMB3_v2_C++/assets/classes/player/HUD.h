#pragma once

#include "Player.h"

class HUD : public Entity {
private:
	static Texture* _hudTexture;

	const unsigned int _LIVES_MAX_LENGTH = 2;
	const unsigned int _COINS_MAX_LENGTH = 2;
	const unsigned int _SCORE_MAX_LENGTH = 7;
	const unsigned int _TIME_MAX_LENGTH = 3;

	std::string _animationName;

	std::vector<std::string> _lives;
	std::vector<std::string> _coins;
	std::vector<std::string> _items;
	std::vector<std::string> _speedGauge;
	std::vector<std::string> _score;
	std::vector<std::string> _timeLeft;

	Player* _player;

	void _ParseLives();
	void _ParseCoins();
	void _ParseItems();
	void _ParseSpeedGauge();
	void _ParseScore();
	void _ParseTimeLeft(DWORD);
	void _ParseSceneEnd();

	void _ParseSprites(std::string) override;

public:
	HUD(Player*);
	~HUD();

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;
	
	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;
	
	//Scene time
	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	void Render() override;

	void Release() override;
};
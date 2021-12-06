#pragma once

class Player;
class ScorePopUp :public Entity {
private:
	static Texture* _scoreTexture;

	unsigned int _index;
	unsigned int _score;
	std::vector<unsigned int> _scores;

	bool _dontReset;

	Player* _player;

	DWORD _floatStart;
	DWORD _floatTime;

	void _ParseSprites(std::string) override;

public:
	ScorePopUp(Player*);
	~ScorePopUp();

	//Get its objectType to compute the score
	void GetEntity(Entity*);

	bool IsFloating() const;
	void StartFloatTimer();

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
#pragma once

class Player;
class Tail : public Entity {
private:
	const float _TAIL_WIDTH = 34.0f;
	const float _TAIL_HEIGHT = 6.0f;

	Player* _player;

	void _ParseSprites(std::string) override;

public:
	Tail(Player*);
	~Tail();

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;
	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Release() override;
};
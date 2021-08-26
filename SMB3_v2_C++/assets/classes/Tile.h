#pragma once

class Tile : public Entity {
private:
	void _ParseSprites(std::string) override;

public:
	void AddHitbox(RECTF);

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;
	void Release() override;
};
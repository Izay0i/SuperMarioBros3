#pragma once

class Goomba : public Entity {
protected:
	enum class _State {
		FLY = 2,
		WALK = 1,
		DIE = 0
	};

	static Texture* _goombaTexture;

	std::string _animationName;

	_State _state;

	void _ParseSprites(std::string) override;

public:
	std::string animationName;

	Goomba();
	virtual ~Goomba();

	RECTF GetBoundingBox(int = 0) const override;

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;
	
	void Release() override;
};
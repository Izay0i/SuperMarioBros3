#pragma once

struct CeilStruct {
	RECT spriteBound;
	float offsetX;
	float offsetY;
};

class MovingCeiling : public Entity {
private:
	static Texture* _ceilingTexture;
	static D3DX10_SPRITE _sprite;

	const float _MAX_HEIGHT = 112.0f;

	const unsigned int _SPRITE_WIDTH = 16;
	const unsigned int _SPRITE_HEIGHT = 16;

	std::vector<CeilStruct> _activeSprites;
	std::vector<CeilStruct> _sprites;

	DWORD _idleStart;
	DWORD _idleTime;

	D3DXVECTOR2 _originalPos;

	D3DXMATRIX _scaleMatrix;

	void _ParseSprites(std::string) override;
	void _ParseMovingCeiling(std::string);

	bool _IsInViewport(CeilStruct, RECTF);

	void _ScaleSprite(const RECT&);

public:
	bool isMoving;

	MovingCeiling();
	~MovingCeiling();

	bool IsIdling() const;
	void StartIdleTimer();

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
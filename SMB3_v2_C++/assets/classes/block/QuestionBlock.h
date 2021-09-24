#pragma once

class QuestionBlock : public Entity {
private:
	enum class _State {
		ROTATE = 2,
		PUSHED = 1
	};
	
	const float _MAX_Y_OFFSET = 0.4f;

	static Texture* _questionTexture;

	_State _state;

	D3DXVECTOR2 _originalPos;

	void _ParseSprites(std::string) override;

public:
	QuestionBlock();
	~QuestionBlock();

	RECTF GetBoundingBox(int = 0) const override;

	void SetPosition(D3DXVECTOR2) override;

	//Player's health determines which item will spawn
	Entity* SpawnItem(int);

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void TakeDamage() override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
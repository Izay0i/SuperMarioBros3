#pragma once

class BonusItem : public Entity {
private:
	enum class _State {
		ROTATE = 2,
		PICKEDUP = 1
	};

	static Texture* _bonusTexture;

	_State _state;

	std::vector<GameObjectType> _possibleItems;
	GameObjectType _currentItem;

	void _ParseSprites(std::string) override;

public:
	BonusItem();
	~BonusItem();

	GameObjectType GetCurrentItem() const;

	RECTF GetBoundingBox(int = 0) const override;

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
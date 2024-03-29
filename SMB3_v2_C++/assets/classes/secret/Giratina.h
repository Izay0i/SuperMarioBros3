#pragma once

class Giratina : public Entity {
private:
	static Texture* _giratinaTexture;

	DWORD _transitionStart;
	DWORD _transitionTime;

	void _ParseSprites(std::string) override;

public:
	Giratina();
	~Giratina();

	bool IsInTransition() const;
	void StartTransitionTimer();

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
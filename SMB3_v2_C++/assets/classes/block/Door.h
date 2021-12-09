#pragma once

class Door : public Entity {
private:
	static Texture* _doorTexture;

	const float _MAX_HEIGHT = 112.0f;

	DWORD _idleStart;
	DWORD _idleTime;

	D3DXVECTOR2 _originalPos;
	D3DXVECTOR2 _destination;

	void _ParseSprites(std::string) override;

public:
	static bool canTriggerMovingCeiling;

	Door();
	~Door();

	bool IsIdling() const;
	void StartIdleTimer();

	void SetPosition(D3DXVECTOR2) override;
	D3DXVECTOR2 GetDestination() const;

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
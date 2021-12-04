#pragma once

class Rotodisc : public Entity {
private:
	static Texture* _rotoTexture;

	//Clockwise: -1, Counterclockwise: 1
	int _direction;

	float _radius;
	//I can be your angle or yuor devil
	float _angle;
	
	DWORD _roundTripTime;

	D3DXVECTOR2 _centerPoint;

	void _ParseSprites(std::string) override;

public:
	Rotodisc();
	~Rotodisc();

	void ParseData(std::string, Texture*&, std::vector<std::string> = std::vector<std::string>()) override;

	void HandleStates() override;
	void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) override;

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;

	void Release() override;
};
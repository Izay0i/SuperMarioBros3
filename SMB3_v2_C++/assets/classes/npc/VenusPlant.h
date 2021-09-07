#pragma once

class Fireball;
class VenusPlant : public PiranaPlant {
public:
	VenusPlant();
	~VenusPlant();

	Fireball* SpawnFireball();

	void Update(DWORD, std::vector<Entity*>* = nullptr, std::vector<Entity*>* = nullptr, Grid* = nullptr) override;
	void Render() override;
};
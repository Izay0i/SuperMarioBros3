#pragma once

#include <fstream>

//#include "../Entity.h"
//
//class BonusItem : public Entity {
//private:
//	enum class ItemState {
//		ROTATE,
//		PICKEDUP
//	};
//
//	const static int MAX_FILE_LINE = 1024;
//
//	void CheckCollision(Entity*, Entity*) override {}
//
//	void ParseSprites(std::string);
//	void ParseHitboxes(std::string);
//
//public:
//	BonusItem();
//
//	void ParseData(std::string);
//
//	void SetPosition(D3DXVECTOR3) override {}
//	D3DXVECTOR3 GetPosition() override {}
//
//	void SetRotation(D3DXVECTOR2) override {}
//	D3DXVECTOR2 GetRotation() override {}
//
//	void SetTranslation(D3DXVECTOR2) override {}
//	D3DXVECTOR2 GetTranslation() override {}
//
//	void SetScale(D3DXVECTOR2) override {}
//	D3DXVECTOR2 GetScale() override {}
//
//	void Update(DWORD) override;
//	void Render() override;
//
//	void Release() override;
//};
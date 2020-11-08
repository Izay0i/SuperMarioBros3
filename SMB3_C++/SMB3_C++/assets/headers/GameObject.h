#pragma once

#include <d3dx9.h>

class GameObject {
protected:
	int objectID;
	
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 distance;

	D3DXVECTOR3 position;
	D3DXVECTOR2 rotation;
	D3DXVECTOR2 translation;
	D3DXVECTOR2 scale;

	D3DXMATRIX matrix;

	static LPDIRECT3DDEVICE9 directDevice;
	static LPD3DXSPRITE spriteHandler;
	
public:
	virtual ~GameObject() {}

	static void SetDevice(LPDIRECT3DDEVICE9&);
	static LPDIRECT3DDEVICE9 GetDevice();

	static void SetSpriteHandler(LPD3DXSPRITE&);
	static LPD3DXSPRITE GetSpriteHandler();

	void SetObjectID(int id) { objectID = id; }
	int GetObjectID() { return objectID; }

	virtual	void SetPosition(D3DXVECTOR3 pos) { position = pos; }
	virtual	D3DXVECTOR3 GetPosition() { return position; }

	virtual	void SetRotation(D3DXVECTOR2 rot) { rotation = rot; }
	virtual	D3DXVECTOR2 GetRotation() { return rotation; }

	virtual	void SetTranslation(D3DXVECTOR2 trans) { translation = trans; }
	virtual	D3DXVECTOR2 GetTranslation() { return translation; }

	virtual	void SetScale(D3DXVECTOR2 sc) { scale = sc; }
	virtual	D3DXVECTOR2 GetScale() { return scale; }

	virtual void Update(DWORD) = 0;
	virtual void Render() = 0;

	virtual void Release() = 0;
};
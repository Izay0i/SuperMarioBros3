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
	
public:
	virtual ~GameObject() {}

	void SetObjectID(int id) { objectID = id; }
	int GetObjectID() { return objectID; }

	virtual void Update(DWORD) = 0;
	virtual void Render() = 0;

	virtual void Release() = 0;
};

typedef struct RectFloat {
	float left;
	float top;
	float right;
	float bottom;

	RectFloat() : left(0), top(0), right(0), bottom(0) {}
	RectFloat(float l, float t, float r, float b) : left(l), top(t), right(r), bottom(b) {}
} RECTF;
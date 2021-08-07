#pragma once

#include "GlobalUtil.h"
#include "Camera.h"

#include <string>
#include <vector>

class Sprite {
private:
	unsigned int _totalFrames, _currentFrame;
	int _animationSpeed;

	std::vector<RECT> _bounds;

	Camera* _cameraInstance;

	DWORD _lastFrameTime;

	LPDIRECT3DTEXTURE9 _spriteTexture;
	D3DCOLOR _colorKey;

public:
	//Sprite texture, sprite bound, total frames, animation speed, color key
	Sprite(LPDIRECT3DTEXTURE9&, RECT, unsigned int, int, D3DCOLOR);
	~Sprite();

	void AddSpriteBound(RECT);
	//Sprite position, scale, alpha
	void DrawSprite(D3DXVECTOR2, D3DXVECTOR2, unsigned int);
	
	//Should be the first to call when releasing resources
	void Release();
};


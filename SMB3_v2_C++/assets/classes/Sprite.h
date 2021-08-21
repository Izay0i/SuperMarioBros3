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

public:
	//Sprite texture, sprite bound, total frames, animation speed
	Sprite(const LPDIRECT3DTEXTURE9&, RECT, unsigned int, int);
	~Sprite();

	void AddSpriteBound(RECT);
	//Sprite position, scale, alpha
	void DrawSprite(D3DXVECTOR2, D3DXVECTOR2, unsigned int);
	
	void Release();
};


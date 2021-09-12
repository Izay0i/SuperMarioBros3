#pragma once

#include "Camera.h"

#include <vector>

class Sprite {
private:
	unsigned int _totalFrames, _currentFrame;
	int _animationSpeed;

	std::vector<RECT> _bounds;

	Camera* _cameraInstance;

	DWORD _lastFrameTime;

	//Direct3D 10
	Texture* _texture;

	D3DXMATRIX _scaleMatrix;
	D3DX10_SPRITE _sprite;
	//REMOVED
	//LPDIRECT3DTEXTURE9 _spriteTexture;
	//END

	//Scales sprite based on the texture position
	void _ScaleSprite(const RECT&, D3DXVECTOR2, unsigned int);

public:
	//Sprite texture, sprite bound, total frames, animation speed
	//Direct3D 10
	Sprite(Texture*&, RECT, unsigned int, int);
	//CHANGED
	//Sprite(const LPDIRECT3DTEXTURE9&, RECT, unsigned int, int);
	//END
	~Sprite();

	void AddSpriteBound(RECT);
	//Sprite position, scale, alpha
	void DrawSprite(D3DXVECTOR2, D3DXVECTOR2, unsigned int);
	
	void Release();
};


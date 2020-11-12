#include "../headers/AnimatedSprite.h"

LPDIRECT3DDEVICE9 AnimatedSprite::directDevice = nullptr;
LPD3DXSPRITE AnimatedSprite::spriteHandler = nullptr;

void AnimatedSprite::SetDevice(LPDIRECT3DDEVICE9& dev) { 
	if (!directDevice) {
		directDevice = dev;
	}
}

LPDIRECT3DDEVICE9 AnimatedSprite::GetDevice() { 
	return directDevice; 
}

void AnimatedSprite::SetSpriteHandler(LPD3DXSPRITE& handler) { 
	if (!spriteHandler) {
		spriteHandler = handler;
	}
}

LPD3DXSPRITE AnimatedSprite::GetSpriteHandler() { 
	return spriteHandler; 
}

Sprite* AnimatedSprite::GetAnimation(std::string animName) {
	return sprites[animName];
}

bool AnimatedSprite::HasAnimation(std::string animName) {
	return sprites.find(animName) != sprites.end();
}

void AnimatedSprite::ParseSprites(std::string line, LPDIRECT3DTEXTURE9 texture, D3DCOLOR colorKey) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 5) {
		return;
	}

	Sprite::SetDevice(directDevice);
	Sprite::SetSpriteHandler(spriteHandler);

	RECT bound;
	bound.left = atoi(tokens.at(1).c_str());
	bound.top = atoi(tokens.at(2).c_str());
	bound.right = atoi(tokens.at(3).c_str()) + 1;
	bound.bottom = atoi(tokens.at(4).c_str()) + 1;

	if (!HasAnimation(tokens.at(0))) {
		int totalFrames = atoi(tokens.at(5).c_str());
		float animSpeed = atof(tokens.at(6).c_str());
		sprites.insert(std::make_pair(tokens.at(0), new Sprite(texture, bound, totalFrames, animSpeed, colorKey)));

		char debugStr[100];
		sprintf_s(debugStr, "[ANIMATED SPRITE] Added animation: %s\n", tokens.at(0).c_str());
		OutputDebugStringW(Util::ToLPCWSTR(debugStr));
	}
	else {
		sprites[tokens.at(0)]->AddBound(bound);
	}
}

void AnimatedSprite::AddBound(RECT bound, std::string animName) {
	sprites[animName]->AddBound(bound);
}

void AnimatedSprite::PlayAnimation(std::string animName, D3DXVECTOR3 position, D3DXVECTOR2 scale, D3DXVECTOR2 center) {
	if (!HasAnimation(animName)) {
		char debugStr[100];
		sprintf_s(debugStr, "[ANIMATED SPRITE] Animation %s does not exist\n", animName.c_str());
		OutputDebugStringW(Util::ToLPCWSTR(debugStr));
		return;
	}

	sprites[animName]->Draw(position, scale, center);
}
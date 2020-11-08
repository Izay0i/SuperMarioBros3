#include "../headers/GameObject.h"

LPDIRECT3DDEVICE9 GameObject::directDevice = nullptr;
LPD3DXSPRITE GameObject::spriteHandler = nullptr;

void GameObject::SetDevice(LPDIRECT3DDEVICE9& dev) { 
	if (!directDevice) { 
		directDevice = dev; 
	} 
}

LPDIRECT3DDEVICE9 GameObject::GetDevice() { 
	return directDevice; 
}

void GameObject::SetSpriteHandler(LPD3DXSPRITE& handler) { 
	if (!spriteHandler) { 
		spriteHandler = handler; 
	} 
}

LPD3DXSPRITE GameObject::GetSpriteHandler() { 
	return spriteHandler; 
}
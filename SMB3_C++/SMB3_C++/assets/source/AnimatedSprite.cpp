#include "../headers/AnimatedSprite.h"

Sprite* AnimatedSprite::GetAnimation(std::string animName) {
	return sprites[animName];
}

bool AnimatedSprite::HasAnimation(std::string animName) {
	return sprites.find(animName) != sprites.end();
}

void AnimatedSprite::ParseSprites(std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 5) {
		return;
	}

	RECT bound;
	bound.left = atoi(tokens.at(1).c_str());
	bound.top = atoi(tokens.at(2).c_str());
	bound.right = atoi(tokens.at(3).c_str()) + 1;
	bound.bottom = atoi(tokens.at(4).c_str()) + 1;

	if (!HasAnimation(tokens.at(0))) {

		int index = atoi(tokens.at(7).c_str());
		std::string texturePath = SceneManager::GetInstance()->GetCurrentScene()->GetTexturePath(index);
		D3DCOLOR colorKey = SceneManager::GetInstance()->GetCurrentScene()->GetTextureColorKey(index);

		int totalFrames = atoi(tokens.at(5).c_str());
		float animSpeed = atof(tokens.at(6).c_str());
		sprites[tokens.at(0)] = new Sprite(texturePath, bound, totalFrames, animSpeed, colorKey);

		char debugStr[100];
		sprintf_s(debugStr, "Added animation: %s\n", tokens.at(0).c_str());
		OutputDebugStringW(Util::ToLPCWSTR(debugStr));
	}
	else {
		sprites[tokens.at(0)]->AddBound(bound);
	}
}

void AnimatedSprite::AddBound(RECT bound, std::string animName) {
	sprites[animName]->AddBound(bound);
}

void AnimatedSprite::PlayAnimation(std::string animName, D3DXVECTOR3 position, D3DXVECTOR2 scale) {
	if (!HasAnimation(animName)) {
		char debugStr[100];
		sprintf_s(debugStr, "Animation %s does not exist\n", animName.c_str());
		OutputDebugStringW(Util::ToLPCWSTR(debugStr));
		return;
	}

	sprites[animName]->Draw(position, scale);
}
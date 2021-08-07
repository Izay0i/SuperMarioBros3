#pragma once

#include "GlobalUtil.h"
#include "GameObject.h"

class Camera : public GameObject {
private:
	static Camera* _cameraInstance;

	unsigned int _cameraWidth, _cameraHeight;
	std::vector<RECTF> _cameraBounds;

	Camera();
	~Camera();

public:
	static Camera* GetInstance();

	void SetCameraWidth(unsigned int);
	unsigned int GetCameraWidth() const;
	void SetCameraHeight(unsigned int);
	unsigned int GetCameraHeight() const;

	RECTF GetViewport() const;
	RECTF GetCameraBound(int = 0) const;
	void AddCameraBound(RECTF);

	void Update(DWORD, std::vector<GameObject*>* = nullptr) override;
	
	void Release() override;
};
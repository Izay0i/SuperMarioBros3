#include "../headers/Camera.h"

GameObject* Camera::cameraInstance = nullptr;

Camera* Camera::GetInstance() {
	if (!cameraInstance) {
		cameraInstance = new Camera;
	}
	return static_cast<Camera*>(cameraInstance);
}

void Camera::Release() {
	if (cameraInstance) {
		delete cameraInstance;
		cameraInstance = nullptr;
	}
}
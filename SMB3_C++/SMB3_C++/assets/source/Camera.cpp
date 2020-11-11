#include "../headers/Camera.h"

GameObject* Camera::cameraInstance = nullptr;

Camera* Camera::GetInstance() {
	if (!cameraInstance) {
		cameraInstance = new Camera;
	}
	return static_cast<Camera*>(cameraInstance);
}

void Camera::SetPosition(D3DXVECTOR3 pos) {
	position = pos;
}

D3DXVECTOR3 Camera::GetPosition() const {
	return position;
}

void Camera::SetWidth(int w) {
	camWidth = w;
}

int Camera::GetWidth() const {
	return camWidth;
}

void Camera::SetHeight(int h) {
	camHeight = h;
}

int Camera::GetHeight() const {
	return camHeight;
}

void Camera::Release() {
	if (cameraInstance) {
		delete cameraInstance;
		cameraInstance = nullptr;
	}
}
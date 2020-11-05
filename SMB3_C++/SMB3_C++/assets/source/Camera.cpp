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

RECTF Camera::GetBound() const {
	RECTF bound;
	bound.left = position.x - camWidth / 2;
	bound.top = position.y - camHeight / 2;
	bound.right = bound.left + camWidth;
	bound.bottom = bound.top + camHeight;

	return bound;
}

void Camera::Release() {
	if (cameraInstance) {
		delete cameraInstance;
		cameraInstance = nullptr;
	}
}
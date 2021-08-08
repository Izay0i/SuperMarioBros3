#include "../headers/Camera.h"

Camera* Camera::cameraInstance = nullptr;

Camera* Camera::GetInstance() {
	if (!cameraInstance) {
		cameraInstance = new Camera;
	}
	return cameraInstance;
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

RECTF Camera::GetViewport() const {
	RECTF viewPort;
	viewPort.left = position.x;
	viewPort.top = position.y;
	viewPort.right = position.x + Util::SCREEN_WIDTH;
	viewPort.bottom = position.y + Util::SCREEN_HEIGHT;
	return viewPort;
}

RECTF Camera::GetLimit(int ind) const {
	return limits.at(ind);
}

void Camera::AddLimit(RECTF limitRect) {
	limits.push_back(limitRect);
}

void Camera::Update(DWORD delta, std::vector<GameObject*>* objects) {
	position.y = 15.0f;

	velocity.x = 0.035f;
	GameObject::Update(delta);
	position += distance;
}

void Camera::Release() {
	if (cameraInstance) {
		limits.clear();

		delete cameraInstance;
		cameraInstance = nullptr;
	}
}
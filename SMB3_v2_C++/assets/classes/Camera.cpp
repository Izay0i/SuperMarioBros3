#include "GlobalUtil.h"
#include "Camera.h"

Camera* Camera::_cameraInstance = nullptr;

Camera::Camera() {}

Camera::~Camera() {}

Camera* Camera::GetInstance() {
	if (_cameraInstance == nullptr) {
		_cameraInstance = new Camera;
	}
	return _cameraInstance;
}

void Camera::SetCameraWidth(unsigned int cameraWidth) {
	_cameraWidth = cameraWidth;
}

unsigned int Camera::GetCameraWidth() const {
	return _cameraWidth;
}

void Camera::SetCameraHeight(unsigned int cameraHeight) {
	_cameraHeight = cameraHeight;
}

unsigned int Camera::GetCameraHeight() const {
	return _cameraHeight;
}

RECTF Camera::GetViewport() const {
	RECTF viewPort;
	viewPort.left = _position.x - GlobalUtil::SCREEN_WIDTH * 0.2f;
	viewPort.top = _position.y - GlobalUtil::SCREEN_HEIGHT * 0.2f;
	viewPort.right = _position.x + GlobalUtil::SCREEN_WIDTH * 1.2f;
	viewPort.bottom = _position.y + GlobalUtil::SCREEN_HEIGHT * 1.2f;
	return viewPort;
}

RECTF Camera::GetCameraBound(int index) const {
	return _cameraBounds.at(index);
}

void Camera::AddCameraBound(RECTF cameraBound) {
	_cameraBounds.emplace_back(cameraBound);
}

void Camera::Update(DWORD deltaTime, std::vector<GameObject*>* collidableObjects) {
	/*_position.y = 15.0f;
	_velocity.x = 0.035f;
	GameObject::Update(deltaTime);
	_position += _distance;*/
}

void Camera::Release() {
	_cameraBounds.clear();

	if (_cameraInstance) {
		delete _cameraInstance;
		_cameraInstance = nullptr;
	}
}
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

Camera::Camera() {
	_viewMatrix = lookAt(vec3(0.0f, 0.0f, 5.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	_lastFrameTime = 0.0f;
}

Camera::~Camera() {
}

void Camera::TakeDownCamera() {
	delete mainCamera;
}

bool Camera::Initialize() {
	if (mainCamera == NULL) {
		Camera *mainCam = new Camera();
		mainCamera = mainCam;
		return true;
	}
	return false;
}

mat4 Camera::GetView() {
	return mainCamera->_viewMatrix;
}

void Camera::Update(float time) {
	float deltaTime = time - mainCamera->_lastFrameTime;
	float rotAngle = k_mars_rotation_speed * deltaTime;
	mainCamera->_viewMatrix = rotate(mainCamera->_viewMatrix, rotAngle, vec3(0, 1, 0));
	mainCamera->_lastFrameTime = time;
}
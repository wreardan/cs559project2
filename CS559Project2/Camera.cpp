#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

Camera::Camera() {
	_viewMatrix = mat4(1.0f);
	_viewMatrix = translate(_viewMatrix, vec3(0.0f, 0.0f, -50.0f));
	//lookAt(vec3(0.0f, 0.0f, 5.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	_lastFrameTime = 0.0f;
	_type = Camera::Type::normal;
	//_viewMatrix = rotate(_viewMatrix, 30.0f, vec3(0, 1, 0));
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

Camera::Type Camera::GetCameraType() {
	return mainCamera->_type;
}

void Camera::SetCameraType(Camera::Type type) {
	mainCamera->_viewMatrix = mat4(1.0f); //reset view matrix
	if (type == Camera::Type::chase) {
		mainCamera->_viewMatrix = rotate(mainCamera->_viewMatrix, 45.0f, vec3(0.0f, 1.0f, 0.0f));
		mainCamera->_viewMatrix = translate(mainCamera->_viewMatrix, vec3(0.0f, 0.0f, -1.5f));
		mainCamera->_type = type;
	} else if (type == Camera::Type::normal) {
		mainCamera->_viewMatrix = translate(mainCamera->_viewMatrix, vec3(0.0f, 0.0f, -5.5f));
		mainCamera->_type = type;
	}
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
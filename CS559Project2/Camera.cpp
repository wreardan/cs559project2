#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#define PI (3.14159f)

using namespace glm;

Camera::Camera() {
	scalar = 12.0f;
	rotation_speed = 20.0f;
}

Camera::~Camera() {
}

void Camera::TakeDownCamera() {
}

bool Camera::Initialize() {
	return true;
}

Camera::Type Camera::GetCameraType() {
	return type;
}

void Camera::SetCameraType(Camera::Type type) {
	this->type = type;
}

mat4 Camera::GetView() {
	return viewMatrix;
}

void Camera::Update(float time) {

	float deltaTime = time - lastFrameTime;
	float rotAngle = time*rotation_speed;
	mat4 rotx, roty, rotz;

	rotz = rotate(mat4(1.0f), 0.0f, vec3(0.0f, 0.0f, 1.0f));
	roty = rotate(mat4(1.0f), 0.0f, vec3(0.0f, 1.0f, 0.0f));
	rotx = rotate(mat4(1.0f), rotAngle, vec3(1.0f, 0.0f, 0.0f));
	
	switch(type)
	{
	case Type::chase:
		position = vec3(roty * rotx * vec4(0.0f, 0.0f, -scalar, 1.0f));
		facing = vec3(roty * rotx * vec4(0.0f, scalar*1.5, 0.0f, 1.0f));
		up = vec3(roty * rotx * vec4(0.0f, 0.0f, -1.0f, 1.0f));
		break;
	case Type::normal:
		position = vec3(roty * rotx * vec4(0.0f, 0.0f, -4.0f*scalar, 1.0f));
		facing = vec3(0.0f, 0.0f, 0.0f);
		up = vec3(0.0f, 1.0f, 0.0f);
		break;
	default:
		assert(false);
		break;
	}

	viewMatrix = lookAt(position, facing, up);

	lastFrameTime = time;
}
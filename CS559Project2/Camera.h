#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Camera
{
protected:
	glm::mat4 viewMatrix;
	glm::vec3 position;
	glm::vec3 facing;
	glm::vec3 up;

public:
	Camera();
	~Camera(void);

//Public Variables
	enum Type {
		normal,
		chase
	};
	float scalar;
	float rotation_speed;
	Camera::Type type;

	//Methods
	void SetCameraType(Camera::Type type);
	Camera::Type GetCameraType();
	glm::mat4 GetView();
	bool Initialize();
	void TakeDownCamera();
	void Update(float time);
	float lastFrameTime;
};
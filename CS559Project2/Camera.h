#pragma once
#include <glm/glm.hpp>

#define k_mars_rotation_speed (20.0f)
class Camera
{
public:
	Camera();
	~Camera(void);

//Global Variables
	enum Type {
		normal,
		chase
	};
	Camera::Type _type;
	static void SetCameraType(Camera::Type type);
	static Camera::Type GetCameraType();
	static glm::mat4 GetView();
	static bool Initialize();
	static void TakeDownCamera();
	static void Update(float time);
	glm::mat4 _viewMatrix;
	float _lastFrameTime;	
};

static Camera *mainCamera;
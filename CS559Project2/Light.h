#pragma once

#include <glm/glm.hpp>
#include <vector>

class Light
{
protected:
	glm::vec3 position;
public:
	Light(void);
	void SetPosition(glm::vec3 & position);
	glm::vec3 GetPosition();
	~Light(void);
};

class Lights
{
public:
	Lights();
	void Add(Light & light);

	std::vector<Light> lights;
	glm::mat4 cameraMatrix;
	glm::vec3 GetPosition(int i);
};
#pragma once

#include <glm/glm.hpp>
#include <vector>

class Light
{
protected:
	glm::vec4 position;
public:
	Light(void);
	void SetPosition(glm::vec4 & position);
	glm::vec4 GetPosition();
	~Light(void);
};

class Lights
{
public:
	Lights();
	void Add(Light & light);

	std::vector<Light> lights;
	glm::mat4 cameraMatrix;
	glm::vec4 GetPosition(int i);
};
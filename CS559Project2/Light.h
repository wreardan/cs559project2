#pragma once

#include <glm/glm.hpp>

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


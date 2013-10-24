#include "Light.h"


Light::Light(void)
{
}


Light::~Light(void)
{
}

void Light::SetPosition(glm::vec3 & position)
{
	this->position = position;
}

glm::vec3 Light::GetPosition()
{
	return position;
}

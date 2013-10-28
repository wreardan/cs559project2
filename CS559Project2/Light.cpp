#include "Light.h"

using namespace std;
using namespace glm;

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


//Light(s)
Lights::Lights()
{
}

void Lights::Add(Light & light)
{
	this->lights.push_back(light);
}

glm::vec3 Lights::GetPosition(int i)
{
	vec3 pos = vec3(cameraMatrix * vec4(lights[i].GetPosition(), 1.0f));
	return pos;
}

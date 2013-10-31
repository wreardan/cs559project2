#include "Light.h"

using namespace std;
using namespace glm;

Light::Light(void)
{
}


Light::~Light(void)
{
}

void Light::SetPosition(glm::vec4 & position)
{
	this->position = position;
}

glm::vec4 Light::GetPosition()
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

glm::vec4 Lights::GetPosition(int i)
{
	vec4 pos = cameraMatrix * lights[i].GetPosition();
	return pos;
}

#pragma once
#include "mesh.h"
#include <glm/glm.hpp>

using namespace glm;

class Ship :
	public Mesh
{
public:
	Ship(void);
	~Ship(void);
	void BuildShip();
	virtual void Draw(const mat4 & projection, mat4 view, const ivec2 & size, const float time);
};


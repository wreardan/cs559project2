#pragma once
#include "Sphere.h"
#include <glm/glm.hpp>

using namespace glm;

class Ship
{
protected:
	Sphere sphere;
	float wing_angle;
	bool draw_normals;
public:
	Ship(void);
	~Ship(void);
	void BuildPrimitive();
	bool Initialize();
	void TakeDown();
	void StepShader();
	void DrawWing(const mat4 & projection, mat4 & view, const ivec2 & size, const float time, int wing_number);
	void DrawChassis(const mat4 & projection, mat4 & view, const ivec2 & size, const float time);
	virtual void Draw(const mat4 & projection, mat4 & view, const ivec2 & size, const float time);
	void EnableNormals(bool dn);
};


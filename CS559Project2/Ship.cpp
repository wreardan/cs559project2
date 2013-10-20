#include "Ship.h"

float Shipangle = 0.0f;

Ship::Ship(void)
{
	this->BuildPrimitive();
}

Ship::~Ship(void)
{
}

void Ship::BuildPrimitive() {
	sphere.BuildMesh(20, 20);
	sphere.BuildPrimitive(2.0f, 20, 20);
	sphere.shader_index = 3;
}


bool Ship::Initialize()
{
	sphere.Initialize(100.0f);
	return true;
}

void Ship::TakeDown()
{
	sphere.TakeDown();
}


void Ship::DrawWing(const mat4 & projection, mat4 & view, const ivec2 & size, const float time)
{
	mat4 mv = view;

	//Draw engine (Cylinder, Disk, Disk)
	

	//Draw Wing (Cylinder/Cube)

	//Draw Weapon (
}

void Ship::Draw(const mat4 & projection, mat4 & view, const ivec2 & size, const float time)
{
	mat4 mv = view;
	//sphere.Draw(projection, mv, size, time);
	//mv = translate(mv, vec3(5.0f, 0.0f, 0.0f));
	//mv = scale(mv, vec3(1.0f, 5.0f, 1.0f));
	//mat3 nm = inverse(transpose(mat3(view*model)));

	//Draw Chassis
	mv = scale(mv, vec3(1.0f, 5.0f, 1.0f));
	sphere.Draw(projection, mv, size, time);

	//Draw Wings
	DrawWing(projection, view, size, time);
}

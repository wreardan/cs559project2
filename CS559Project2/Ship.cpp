#include "Ship.h"

float Shipangle = 0.0f;

Ship::Ship(void)
{
	this->BuildPrimitive();
}

Ship::~Ship(void)
{
}

void Ship::StepShader()
{
	sphere.shader_index = ++sphere.shader_index % sphere.shaders.size();
}

void Ship::BuildPrimitive() {
	sphere.BuildMesh(20, 20);
	sphere.BuildPrimitive(2.0f, 20, 20);
	sphere.CalculateNormals(20, 20);
	sphere.shader_index = 3;
}

void Ship::EnableNormals(bool dn) {
	this->draw_normals = dn;
	sphere.EnableNormals(dn);
}


bool Ship::Initialize()
{
	sphere.Initialize(100.0f);
	wing_angle = 30.0f;
	return true;
}

void Ship::TakeDown()
{
	sphere.TakeDown();
}

void Ship::DrawChassis(const mat4 & projection, mat4 & view, const ivec2 & size, const float time)
{
	mat4 temp;

	//Main Chassis
	temp = scale(view, vec3(1.0f, 10.0f, 1.0f));
	sphere.Draw(projection, temp, size, time);

	view = translate(view, vec3(0.0f, -16.0f, 0.0f));
}


void Ship::DrawWing(const mat4 & projection, mat4 & view, const ivec2 & size, const float time, int wing_number)
{
	//Rotate to the correct angle
	if(wing_number == 0 || wing_number == 2)
		view = rotate(view, 150.0f - wing_angle, vec3(0.0f, 1.0f, 0.0f));
	else
		view = rotate(view, wing_angle + 30.0f, vec3(0.0f, 1.0f, 0.0f));

	mat4 temp = view;

	//Draw engine (Cylinder + Sphere)
	temp = translate(temp, vec3(2.0f, 0.0f, 0.0f));
	temp = scale(temp, vec3(1.5f, 3.0f, 1.5f));
	sphere.Draw(projection, temp, size, time);

	//Draw Wing (Cylinder/Cube)
	temp = translate(view, vec3(7.5f, 0.0f, 0.0f));
	temp = scale(temp, vec3(5.0f, 3.0f, 0.5f));
	sphere.Draw(projection, temp, size, time);

	//Draw Weapon (Cylinder + Cylinder)
	temp = translate(view, vec3(15.0f, 0.0f, 0.0f));
	temp = scale(temp, vec3(1.0f, 3.0f, 1.0f));
	sphere.Draw(projection, temp, size, time);
	temp = translate(temp, vec3(0.0f, 2.0f, 0.0f));
	temp = scale(temp, vec3(0.5f, 2.0f, 0.5f));
	sphere.Draw(projection, temp, size, time);
}

void Ship::Draw(const mat4 & projection, mat4 & view, const ivec2 & size, const float time)
{
	mat4 mv = view;
	mv = scale(mv, vec3(0.05f, 0.05f, 0.05f));
	//sphere.Draw(projection, mv, size, time);
	//mv = translate(mv, vec3(5.0f, 0.0f, 0.0f));
	//mv = scale(mv, vec3(1.0f, 5.0f, 1.0f));
	//mat3 nm = inverse(transpose(mat3(view*model)));

	//Draw Chassis
	DrawChassis(projection, mv, size, time);

	//Draw Wings
	for(int wing_number = 0; wing_number < 4; wing_number++)
		DrawWing(projection, mv, size, time, wing_number);
}

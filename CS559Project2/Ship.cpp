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
	while(this->sphere.shader_index >= 5 || this->sphere.shader_index == 1) { //skip post-process & flat
		this->sphere.shader_index = ++this->sphere.shader_index % this->sphere.shaders.size();
	}
}

void Ship::BuildPrimitive() {
	sphere.BuildMesh(20, 20);
	sphere.BuildPrimitive(2.0f, 20, 20);
	sphere.CalculateNormals(20, 20);
	sphere.shader_index = 0;
	cylinder.BuildMesh(20, 20);
	cylinder.BuildPrimitive(1.0f, 20, 20, 1.0f);
	cylinder.CalculateNormals(20, 20);
	cylinder.shader_index = 0;
}

void Ship::EnableNormals(bool dn) {
	this->draw_normals = dn;
	sphere.EnableNormals(dn);
	cylinder.EnableNormals(dn);
}


bool Ship::Initialize()
{
	sphere.Initialize(100.0f);
	cylinder.Initialize(100.0f);
	sphere.shader_index = 3;
	cylinder.shader_index = 3;
	wing_angle = 30.0f;
	sphere.LoadShipTexture("metal.jpg");
	cylinder.LoadShipTexture("metal.jpg");
	return true;
}

void Ship::TakeDown()
{
	sphere.TakeDown();
	cylinder.TakeDown();
}

void Ship::DrawChassis(const mat4 & projection, mat4 & view, const ivec2 & size, Lights & lights, const float time)
{
	mat4 temp = view;

	//Main Chassis
	temp = translate(temp, vec3(0.0f, -20.0f, 0.0f));
	temp = scale(temp, vec3(2.0f, 40.0f, 2.0f));
	cylinder.Draw(projection, temp, size, lights, time);

	//Front sphere
	/*temp = translate(view, vec3(0.0f, -19.0f, 0.0f));
	sphere.Draw(projection, temp, size, lights, time);*/

	view = translate(view, vec3(0.0f, -14.0f, 0.0f));
}


void Ship::DrawWing(const mat4 & projection, mat4 & view, const ivec2 & size, Lights & lights, const float time, int wing_number)
{
	//Rotate to the correct angle
	if(wing_number == 0 || wing_number == 2)
		view = rotate(view, 150.0f - wing_angle, vec3(0.0f, 1.0f, 0.0f));
	else
		view = rotate(view, wing_angle + 30.0f, vec3(0.0f, 1.0f, 0.0f));

	mat4 temp = view;

	//Draw engine (Cylinder + Sphere)
	temp = translate(temp, vec3(2.0f, -6.0f, 0.0f));
	temp = scale(temp, vec3(1.5f, 12.0f, 1.5f));
	cylinder.Draw(projection, temp, size, lights, time);

	//Draw Wing (Cylinder/Cube)
	temp = translate(view, vec3(9.0f, -6.0f, 0.0f));
	temp = scale(temp, vec3(6.0f, 12.0f, 1.0f));
	cylinder.Draw(projection, temp, size, lights, time);

	//Draw Weapon (Cylinder + Cylinder)
	temp = translate(view, vec3(15.0f, -6.0f, 0.0f));
	temp = scale(temp, vec3(1.0f, 12.0f, 1.0f));
	cylinder.Draw(projection, temp, size, lights, time);

	temp = translate(temp, vec3(0.0f, 0.9f, 0.0f));
	temp = scale(temp, vec3(0.5f, 1.0f, 0.5f));
	cylinder.Draw(projection, temp, size, lights, time);
}

void Ship::Draw(const mat4 & projection, mat4 & view, const ivec2 & size, Lights & lights, const float time)
{
	mat4 mv = view;
	mv = rotate(mv, 30.0f, vec3(0.0f, 1.0f, 0.0f));
	mv = scale(mv, vec3(0.05f, 0.05f, 0.05f));
	//sphere.Draw(projection, mv, size, time);
	//mv = translate(mv, vec3(5.0f, 0.0f, 0.0f));
	//mv = scale(mv, vec3(1.0f, 5.0f, 1.0f));
	//mat3 nm = inverse(transpose(mat3(view*model)));

	//Draw Chassis
	DrawChassis(projection, mv, size, lights, time);

	//Draw Wings
	for(int wing_number = 0; wing_number < 4; wing_number++)
		DrawWing(projection, mv, size, lights, time, wing_number);
}

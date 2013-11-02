#include "Starfield.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>

using namespace std;

bool Starfield::Initialize()
{
	srand ((unsigned int)time(NULL));
	const float scalar = 0.025f;	//scalar for mars depth data
	ivec2 size = ivec2(100, 100);
	//Create Mesh
	this->BuildMesh(size.x, size.y);
	//Wrap Into Sphere
	this->BuildPrimitive(100.0f, size.x, size.y);
	
	//Modify Mesh Vertices
	int index = 0;
	for(int y = 0; y < size.y; y++) {
		for(int x = 0; x < size.x; x++) {
			//assert(depth >= 0.0f && depth <= 2.0f);
			//vertices[index].position.
			vertices[index].position.x *= rand() % 10 + 1;
			vertices[index].position.y *= rand() % 10 + 1;
			vertices[index].position.z *= rand() % 10 + 1;
			float r = 0.1f / (rand() % 10 + 1);
			float g = 0.1f / (rand() % 10 + 1);
			float b = 0.1f / (rand() % 10 + 1);
			vertices[index].color = vec3(1.0f - r, 1.0f - g, 1.0f - b);
			index++;
		}
	}

	this->CalculateNormals(size.x, size.y);
	super::Initialize(100.0f);
	return true;
}

void Starfield::Update(bool strength) {

	srand ((unsigned int)time(NULL));
	int index = 0;	
	float color_strength;
	if (!strength) { 
		color_strength = 0.5f;
	} else {
		color_strength = 0.8f;
	}
	for(int y = 0; y < 100; y++) {
		for(int x = 0; x < 100; x++) {
			float r = color_strength / (rand() % 10 + 1);
			float g = color_strength / (rand() % 10 + 1);
			float b = color_strength / (rand() % 10 + 1);
			vertices[index].color = vec3(1.0f - r, 1.0f - g, 1.0f - b);
			index++;
		}
	}

	glBindVertexArray(this->vertex_array_handle);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_coordinate_handle);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(VertexAttributesPCNT), &this->vertices[0], GL_STATIC_DRAW);
}

void Starfield::Draw(const mat4 & projection, mat4 view, const ivec2 & size, Lights & lights,  const float time)
{
	glEnable(GL_PROGRAM_POINT_SIZE); //enable to be able to adjust points in shaders
	if (this->GLReturnedError("Mesh::Draw - on entry"))
		return;


	glEnable(GL_DEPTH_TEST);
	mat4 model = glm::mat4(1.0f);
	//model = scale(model, vec3(10, 10, 10));
	mat4 mvp = projection * view * model;
	mat3 nm = inverse(transpose(mat3(view)));

	this->shaders[1]->Use(); //set shader to pure white for stars
	this->GLReturnedError("Mesh::Draw - after use");
	this->shaders[1]->CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(view), value_ptr(mvp), value_ptr(nm));

	this->GLReturnedError("Mesh::Draw - after common setup");
	glBindVertexArray(this->vertex_array_handle);
	glPointSize(2); //change star point size
	glDrawElements(GL_POINTS , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glBindVertexArray(0);
	this->GLReturnedError("Mesh::Draw - after draw");
	glUseProgram(0);

	if (this->draw_normals)
	{
		this->solid_color.Use();
		this->solid_color.CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(view), value_ptr(mvp), value_ptr(nm));
		glBindVertexArray(this->normal_array_handle);
		glDrawElements(GL_LINES , this->normal_indices.size(), GL_UNSIGNED_INT , &this->normal_indices[0]);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	if (this->GLReturnedError("Mesh::Draw - on exit"))
		return;
}

Starfield::Starfield(void)
{
}

Starfield::~Starfield(void)
{

}
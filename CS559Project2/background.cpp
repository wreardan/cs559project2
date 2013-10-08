/*	Perry Kivolowitz - University of Wisconsin - Madison 
	Computer Sciences Department

	A sample hello world like program demonstrating modern
	OpenGL techniques. 

	Created:	2/25/13
	Updates:
*/

#include <iostream>
#include "background.h"

using namespace std;
using namespace glm;

Background::Background() : Object()
{
	vec4 lighter_color(this->MakeColor(0, 80, 0));
	vec4 darker_color = vec4(vec3(lighter_color) / 8.0f, 1.0f);
	this->colors[0] = darker_color;
	this->colors[1] = lighter_color * vec4(1.0f, 0.5f, 1.0f, 1.0f);
	this->colors[2] = lighter_color * vec4(1.0f, 3.0f, 1.0f, 1.0f);
	this->colors[3] = lighter_color;
}


bool Background::Initialize()
{
	if (this->GLReturnedError("Background::Initialize - on entry"))
		return false;

	this->vertices.push_back(VertexAttributesP(vec3(0.0f, 1.0f, 0.0f)));
	this->vertices.push_back(VertexAttributesP(vec3(1.0f, 1.0f, 0.0f)));
	this->vertices.push_back(VertexAttributesP(vec3(1.0f, 0.0f, 0.0f)));
	this->vertices.push_back(VertexAttributesP(vec3(0.0f, 0.0f, 0.0f)));

	this->vertex_indices.push_back(0);
	this->vertex_indices.push_back(3);
	this->vertex_indices.push_back(1);
	this->vertex_indices.push_back(1);
	this->vertex_indices.push_back(3);
	this->vertex_indices.push_back(2);

	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributesP), &this->vertices[0]))
		return false;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesP), (GLvoid *) 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (!this->shader.Initialize("background_gradient.vert", "background_gradient.frag"))
		return false;

	if (this->GLReturnedError("Background::Initialize - on exit"))
		return false;

	return true;
}

void Background::TakeDown()
{
	super::TakeDown();
}

void Background::Draw(const ivec2 & size)
{
	if (this->GLReturnedError("Background::Draw - on entry"))
		return;

	mat4 projection = ortho(0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 10.0f);
	mat4 modelview = translate(mat4(1.0f), vec3(0.0f, 0.0f, -5.5f));
	mat4 mvp = projection * modelview;

	shader.Use();
	shader.CommonSetup(0.0f, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(mat4(1.0f)));
	shader.CustomSetup(this->colors);
	glViewport(0, 0, size.x, size.y);
	glBindVertexArray(this->vertex_array_handle);
	glDrawElements(GL_TRIANGLES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glUseProgram(0);
	glBindVertexArray(0);

	if (this->GLReturnedError("Background::Draw - on exit"))
		return;
}

void Background::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time)
{
	assert(false);
}

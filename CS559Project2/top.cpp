/*	Perry Kivolowitz - University of Wisconsin - Madison 
	Computer Sciences Department

	A sample hello world like program demonstrating modern
	OpenGL techniques. 

	Created:	2/25/13
	Updates:
*/

#include <iostream>
#include "top.h"

using namespace std;
using namespace glm;

Top::Top() : Object()
{
	vec4 lighter_color(MakeColor(255, 69, 0, 1.0f));
	vec4 darker_color = vec4(vec3(lighter_color) * 2.0f / 3.0f, 1.0f);
	this->colors[0] = darker_color;
	this->colors[1] = lighter_color;
	this->shader_index = 0;
}

inline int ColorIndex(int i, int slices)
{
	return (i / (slices / 4)) % 2;
}

inline int PreviousSlice(int i, int slices)
{
	return (i == 0) ? slices - 1 : i - 1;
}

void Top::BuildNormalVisualizationGeometry()
{
	const float normal_scalar = 0.125f;
	for (int j = 1; j <= 3; ++j)
	{
		this->normal_vertices.push_back(VertexAttributesP(this->vertices[this->vertices.size() - j].position));
		this->normal_vertices.push_back(VertexAttributesP(this->vertices[this->vertices.size() - j].position + this->vertices[this->vertices.size() - j].normal * normal_scalar));
		this->normal_indices.push_back(this->normal_vertices.size() - 2);
		this->normal_indices.push_back(this->normal_vertices.size() - 1);
	}
}

void Top::StepShader()
{
	this->shader_index = ++this->shader_index % this->shaders.size();
}

bool Top::Initialize(int slices)
{
	if (this->GLReturnedError("Top::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	if (slices < 0)
		slices = 1;

	slices *= 4;

	mat4 m;

	const vec3 n = normalize(vec3(0.5f, 1.0f, 0.0f));
	const vec4 x_axis(1.0f, 0.0f, 0.0f, 1.0f);
	const vec3 y_axis(0.0f, 1.0f, 0.0f);
	const vec3 apex(0.0f, 0.5f, 0.0f);
	const vec3 nadir(0.0f, -0.5f, 0.0f);
	const float increment =  360.0f / float(slices);
	const vec3 neg_y(1.0f, -1.0f, 1.0f); 

	/*	for each slice:
			compute top triangle geometry
			compute vectors to visualize normals for top triangle (BuildNormalVisualizationGeometry())
			compute bottom triangle geometry
			compute vectors to visualize normals for bottom triangle (BuildNormalVisualizationGeometry())
	*/

	for (int i = 0; i < slices; ++i)
	{
		VertexAttributesPCN cur_vertex , nxt_vertex;
		cur_vertex.position = vec3(m * x_axis);
		cur_vertex.color = vec3(this->colors[ColorIndex(i, slices)]);
		cur_vertex.normal = vec3(m * vec4(n, 1.0f));

		m = rotate(m, increment, y_axis);
		
		nxt_vertex.position = vec3(m * x_axis);
		nxt_vertex.color = vec3(this->colors[1 - ColorIndex(i, slices)]);
		nxt_vertex.normal = vec3(m * vec4(n, 1.0f));
		
		// Top geometry
		this->vertices.push_back(VertexAttributesPCN(apex, vec3(1.0f, 0.0f, 0.0f), normalize(apex)));
		this->vertices.push_back(nxt_vertex);
		this->vertices.push_back(cur_vertex);
	
		this->vertex_indices.push_back(this->vertices.size() - 3);
		this->vertex_indices.push_back(this->vertices.size() - 1);
		this->vertex_indices.push_back(this->vertices.size() - 2);

		this->BuildNormalVisualizationGeometry();

		// Bottom geometry
		this->vertices.push_back(VertexAttributesPCN(nadir, vec3(1.0f, 0.0f, 0.0f), normalize(nadir)));
		this->vertices.push_back(VertexAttributesPCN(nxt_vertex.position, nxt_vertex.color, nxt_vertex.normal * neg_y));
		this->vertices.push_back(VertexAttributesPCN(cur_vertex.position, cur_vertex.color, cur_vertex.normal * neg_y));
	
		this->vertex_indices.push_back(this->vertices.size() - 3);		// Note the winding. Question for reader:
		this->vertex_indices.push_back(this->vertices.size() - 2);		// Why does this differ from the similar
		this->vertex_indices.push_back(this->vertices.size() - 1);		// code a few lines above?

		this->BuildNormalVisualizationGeometry();
	}

	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributesPCN), &this->vertices[0]))
		return false;

	/*	The VertexAttributesPCN class stores vertex attributes: position, color and normal in that order.

		Vertex attributes are stored in an interleaved manner aiding speed of vertex processing.
	*/

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) (sizeof(vec3) * 2));	// Note offset - legacy of older code
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) (sizeof(vec3) * 1));	// Same
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (this->normal_vertices.size() > 0)
	{
		if (!this->PostGLInitialize(&this->normal_array_handle, &this->normal_coordinate_handle, this->normal_vertices.size() * sizeof(VertexAttributesP), &this->normal_vertices[0]))
			return false;

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesP), (GLvoid *) 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	if (!this->shader.Initialize("top_shader.vert", "top_shader.frag"))
		return false;

	if (!this->solid_color.Initialize("solid_shader.vert", "solid_shader.frag"))
		return false;

	if (!this->stripes_model_space.Initialize("stripe_model_space.vert", "stripe_model_space.frag"))
		return false;

	this->shaders.push_back(&this->shader);
	this->shaders.push_back(&this->solid_color);
	this->shaders.push_back(&this->stripes_model_space);

	if (this->GLReturnedError("Background::Initialize - on exit"))
		return false;

	return true;
}

void Top::TakeDown()
{
	this->vertices.clear();
	this->shader.TakeDown();
	this->solid_color.TakeDown();
	super::TakeDown();
}

void Top::Draw(const ivec2 & size)
{
	assert(false);
}

/*	A note about drawing the normals.

	If you scale this object non-uniformly, drawing the normals will
	not be correct. This is because the normals are being stored for
	visualization as geometry. As such, scaling will not be corrected
	by the normal matrix.
*/

/*	A note about the index arrays.

	In this example, the index arrays are unsigned ints. If you know
	for certain that the number of vertices will be small enough, you
	can change the index array type to shorts or bytes. This will have
	the two fold benefit of using less storage and transferring fewer
	bytes.
*/

void Top::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time)
{
	if (this->GLReturnedError("Top::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	modelview = rotate(modelview, time * 30.0f, vec3(1.0f, 0.0f, 0.0f));
	modelview = rotate(modelview, time * 120.0f, vec3(0.0f, 1.0f, 0.0f));
	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));

	this->shaders[this->shader_index]->Use();
	this->GLReturnedError("Top::Draw - after use");
	this->shaders[this->shader_index]->CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
	this->GLReturnedError("Top::Draw - after common setup");
	glBindVertexArray(this->vertex_array_handle);
	glDrawElements(GL_TRIANGLES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glBindVertexArray(0);
	this->GLReturnedError("Top::Draw - after draw");
	glUseProgram(0);

	if (this->draw_normals)
	{
		this->solid_color.Use();
		this->solid_color.CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
		glBindVertexArray(this->normal_array_handle);
		glDrawElements(GL_LINES , this->normal_indices.size(), GL_UNSIGNED_INT , &this->normal_indices[0]);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	if (this->GLReturnedError("Top::Draw - on exit"))
		return;
}

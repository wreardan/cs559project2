#include "Mesh.h"

using namespace std;
using namespace glm;


Mesh::Mesh(void) : Object()
{
	vec4 lighter_color(MakeColor(255, 69, 0, 1.0f));
	vec4 darker_color = vec4(vec3(lighter_color) * 2.0f / 3.0f, 1.0f);
	this->colors[0] = darker_color;
	this->colors[1] = lighter_color;
	this->shader_index = 0;

	BuildSphere(1.0f, 360, 180);
	//BuildCylinder(1.0f, 1.0f, 20);
}


Mesh::~Mesh(void)
{
}

void Mesh::StepShader()
{
	this->shader_index = ++this->shader_index % this->shaders.size();
}

inline int ColorIndex(int i, int slices)
{
	return (i / (slices / 4)) % 2;
}

inline int PreviousSlice(int i, int slices)
{
	return (i == 0) ? slices - 1 : i - 1;
}

//http://www.opengl.org/wiki/Calculating_a_Surface_Normal
void Mesh::CalculateNormals() {
	unsigned int i;
	for (i = 0; i < (this->vertices.size() * 3 - 2); i++) {
		vec3 u = this->vertices[this->vertex_indices[i+1]].position - this->vertices[this->vertex_indices[i]].position;
		vec3 v = this->vertices[this->vertex_indices[i+2]].position - this->vertices[this->vertex_indices[i]].position;

		vec3 normal;
		normal.x = (u.y *  v.z) - (u.z * v.y);
		normal.y = (u.z * v.x) - (u.x * v.z);
		normal.z = (u.x * v.y) - (u.y * v.x);
		
		normal.y *= -1;
		this->vertices[this->vertex_indices[i]].normal = normal;
	}
}

void Mesh::BuildCylinder(float radius, float height, unsigned int sectors)
{
	const float PI = atan(1.0f)*4;
	const float PI_2 = atan(1.0f)*4/2;

	//Create Vertices
	float const S = 1.0f/(float)(sectors-1);
	unsigned int s;
	for(s = 0; s < sectors; s++) for(int h = -1; h < 2; h+=2) {
		float const y = h * height;
		float const x = cos(2*PI * s * S);
		float const z = sin(2*PI * s * S);

		vec3 vertex = vec3(x * radius, y * radius, z * radius);

		vec3 normal = vec3(x, y, z);

		vec3 color = vec3(1.0f, 0.4f, 0.0f);
		
		vec2 tex = vec2(s*S, 0.0f);

		this->vertices.push_back(VertexAttributesPCNT(vertex, color, normal, tex));
	}

	//Create Indices
	vertex_indices.resize(2 * sectors * 6);
	vector<unsigned int>::iterator i = vertex_indices.begin();
	for(s = 0; s < sectors*2; s++) {
		*i++ = s;
		*i++ = (s+1);
		*i++ = (s+2);
		
		*i++ = (s+1);
		if(s >= sectors*2-2)
			*i++ = 0;
		else
			*i++ = (s+3);
		*i++ = (s+2);
	}
}

//glitch that shows one segment messed up
void Mesh::BuildSphere(float radius, unsigned int sectors, unsigned int rings)
{
	/*
	unsigned int rings = 360;
	unsigned int sectors = 180;
	float radius = 1.0f;*/
	
	const float PI = atan(1.0f)*4;
	const float PI_2 = atan(1.0f)*4/2;

	//Create Vertices
	float const R = 1.0f/(float)(rings-1);
	float const S = 1.0f/(float)(sectors-1);
	unsigned int r, s;
	for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
		float const y = sin( -PI_2 + PI * r * R );
		float const x = cos(2*PI * s * S) * sin( PI * r * R );
		float const z = sin(2*PI * s * S) * sin( PI * r * R );

		vec3 vertex = vec3(x * radius, y * radius, z * radius);

		vec3 normal = vec3(x, -y, z);

		vec3 color = vec3(1.0f, 0.4f, 0.0f);
		
		vec2 tex = vec2(s*S, r*R);

		this->vertices.push_back(VertexAttributesPCNT(vertex, color, normal, tex));
	}

	//Create Indices
	vertex_indices.resize(rings * sectors * 6);
	vector<unsigned int>::iterator i = vertex_indices.begin();
	// NOTE: Isn't this off by one? Also, we may want to duplicate edge verts, or at least fake it, for "wrapping"?
	for(r = 0; r < rings - 1; r++) for(s = 0; s < sectors - 1; s++) { 
		*i++ = r * sectors + s;
		*i++ = r * sectors + (s+1);
		*i++ = (r+1) * sectors + (s+1);
		
		*i++ = (r+1) * sectors + s;
		*i++ = r * sectors + s;
		*i++ = (r+1) * sectors + (s+1);
	}
	*i++ = vertex_indices[0];
	*i++ = vertex_indices[0];
	*i++ = vertex_indices[0];
	*i++ = vertex_indices[0];
	*i++ = vertex_indices[0];
	*i++ = vertex_indices[0];
}

bool Mesh::Initialize(float size)
{
	if (this->GLReturnedError("Mesh::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributesPCNT), &this->vertices[0]))
		return false;

	/*	The VertexAttributesPCN class stores vertex attributes: position, color and normal in that order.

		Vertex attributes are stored in an interleaved manner aiding speed of vertex processing.
	*/

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(vec3) * 2));	// Note offset - legacy of older code
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(vec3) * 1));	// Same
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(vec3) * 3));	// Same
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
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

void Mesh::TakeDown()
{
	this->vertices.clear();
	this->shader.TakeDown();
	this->solid_color.TakeDown();
	super::TakeDown();
}

void Mesh::Draw(const ivec2 & size)
{
	assert(false);
}



void Mesh::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time)
{
	if (this->GLReturnedError("Mesh::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));

	this->shaders[this->shader_index]->Use();
	this->GLReturnedError("Mesh::Draw - after use");
	this->shaders[this->shader_index]->CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
	this->GLReturnedError("Mesh::Draw - after common setup");
	glBindVertexArray(this->vertex_array_handle);
	glDrawElements(GL_TRIANGLES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glBindVertexArray(0);
	this->GLReturnedError("Mesh::Draw - after draw");
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

	if (this->GLReturnedError("Mesh::Draw - on exit"))
		return;
}
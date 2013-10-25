#include "Mesh.h"
#include <IL/il.h>

using namespace std;
using namespace glm;


void Mesh::LoadTexture(char * texture_filename)
{
	texture.Initialize(texture_filename);
}

Mesh::Mesh(void) : Object()
{
	vec4 lighter_color(MakeColor(255, 69, 0, 1.0f));
	vec4 darker_color = vec4(vec3(lighter_color) * 2.0f / 3.0f, 1.0f);
	this->colors[0] = darker_color;
	this->colors[1] = lighter_color;
	this->shader_index = 3;
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

void Mesh::BuildNormalVisualizationGeometry(int index)
{
	const float normal_scalar = 0.125f;
	index+=3;
	if(index >= vertices.size())
		return;
	for (int j = 1; j <= 3; ++j)
	{
		this->normal_vertices.push_back(VertexAttributesP(this->vertices[index - j].position));
		this->normal_vertices.push_back(VertexAttributesP(this->vertices[index - j].position + this->vertices[index - j].normal * normal_scalar));
		this->normal_indices.push_back(this->normal_vertices.size() - 2);
		this->normal_indices.push_back(this->normal_vertices.size() - 1);
	}
}

//http://www.opengl.org/wiki/Calculating_a_Surface_Normal
void Mesh::CalculateNormals(unsigned int columns, unsigned int rows) {
	normal_indices.clear();
	normal_vertices.clear();
	unsigned int r, s, i;
    vec3 curr, right, left, up, down, down_left, down_right, up_left, up_right, normal;
    unsigned int num_columns = columns;
	int index = columns;
    for(r = 1; r < rows-1; r++) {
		for(s = 0; s < columns; s++) {
			//account for last column
			if (s == columns-1) {
				this->vertices[index].normal = this->vertices[index - 1].normal;
				index++;
			} else {
				i = index;
				curr = this->vertices[index].position;
				right = Mesh::GetIndexRight(i, num_columns, r, s); 
				left = Mesh::GetIndexLeft(i, num_columns, r, s);
				up = Mesh::GetIndexUp(i, num_columns, r, s);
				down = Mesh::GetIndexDown(i, num_columns, r, s, rows);
				down_left = Mesh::GetIndexDownLeft(i, num_columns, r, s, rows);
				down_right = Mesh::GetIndexDownRight(i, num_columns, r, s, rows);
				up_left = Mesh::GetIndexUpLeft(i, num_columns, r, s);
				up_right = Mesh::GetIndexRightUp(i, num_columns, r, s);
				vec3 a = cross(left - curr, left - up);
				vec3 b = cross(up - curr, up - up_right);
				vec3 c = cross(curr - right, curr - up_right);
				vec3 d = cross(right - curr, right - down);
				vec3 e = cross(down_left - down, down_left - curr);
				vec3 f = cross(curr - left, curr - down_left);
				normal = (a + b + c + d + e + f) / 6.0f;
				if (dot(normal, normal) != 0) {
						normal = glm::normalize(normal);
				}
				this->vertices[index].normal = normal;
				BuildNormalVisualizationGeometry(index);
				index++;
			}
        }
    }
	// account for zeroeth column
	for(r = 0; r < rows - 1; r++) {
		for(s = 0; s < 1; s++) {
			index = r * columns + s;
			int cheat = r * columns + columns - 1;
			if (s == 0) {
				this->vertices[index].normal = this->vertices[cheat].normal;
				index++;
			}
		}
	}
}


vec3 Mesh::GetIndexLeft(int index, int columns, int r, int s){
	index = index - 1;
	if (index - (r * columns) < 0) {
		index = 0;
	}
	return this->vertices[index].position;
}

vec3 Mesh::GetIndexRight(int index, int columns, int r, int s){
	index = index + 1;
	if ((index - (r * columns) >= columns)) {
		index = columns - 1;
	}
	return this->vertices[index].position;
}

vec3 Mesh::GetIndexUp(int index, int columns, int r, int s){
	index = index - columns;
	if (r < 1) {
		index = index + columns;
	}
	return this->vertices[index].position;
}

vec3 Mesh::GetIndexDown(int index, int columns, int r, int s, int rows){
	index = index + columns;
	if ((((index - s) / columns) >= rows)) {
		index = index - columns;
	}
	return this->vertices[index].position;
}

vec3 Mesh::GetIndexUpLeft(int index, int columns, int r, int s){
	index = (index - 1) - columns;
	if (index - (r * columns) < 0) {
		index = 0;
	}
	if (r < 1) {
		index = index + columns;
	}
	return this->vertices[index].position;
}

vec3 Mesh::GetIndexRightUp(int index, int columns, int r, int s){
	index = (index + 1) - columns;
	if ((index - (r * columns) >= columns)) {
		index = columns - 1;
	}
	if (r < 1) {
		index = index + columns;
	}
	return this->vertices[index].position;
}

vec3 Mesh::GetIndexDownLeft(int index, int columns, int r, int s, int rows){
	index = (index - 1) + columns;
	if (index - (r * columns) <= 0) {
		index = index + 1;
	}
	if ((((index - s) / columns) >= rows-1)) {
		index = index - columns;
	}
	return this->vertices[index].position;
}

vec3 Mesh::GetIndexDownRight(int index, int columns, int r, int s, int rows){
	index = (index + 1) + columns;
	if ((index - (r * columns) >= columns)) {
		index = columns - 1;
	}
	if ((((index - s) / columns) >= rows)) {
		index = index - columns;
	}
	return this->vertices[index].position;
}

void Mesh::BuildMesh(unsigned int columns, unsigned int rows)
{
	// Create planar mesh
	unsigned int r, s;

	int index = 0;
	for(r = 0; r < rows; r++) {
		for(s = 0; s < columns; s++) {
			float const x = s;
			float const y = r;
			float const z = 0;

			vec3 vertex = vec3(x, y , z);

			vec3 normal = vec3(0, 0, -1);

			vec3 color = vec3(1.0f, 0.4f, 0.0f);
		
			vec2 tex = vec2(float(s) / (columns-1), float(r) / (rows-1));

			this->vertices.push_back(VertexAttributesPCNT(vertex, color, normal, tex));
		}
	}


	// Wind triangles
	vertex_indices.resize(rows * columns * 6);
	vector<unsigned int>::iterator i = vertex_indices.begin();
	//int triangle_number = 0;
	for(r = 0; r < rows - 1; r++) {
        for(s = 0; s < columns - 1; s++) {
			//printf("Triangle Number: %i -- \n", triangle_number);
            *i++ = r * columns + (s+1);
			//printf("	Row: %i | Column: %i | Index: %i\n", r, s, *i);
            *i++ = (r) * columns + (s);
			//printf("	Row: %i | Column: %i | Index: %i\n", r, s, *i);
            *i++ = (r+1) * columns + s;
			//printf("	Row: %i | Column: %i | Index: %i\n", r, s, *i);
			//printf("\n");
            //triangle_number++;

			//printf("Triangle Number: %i -- \n", triangle_number);
            *i++ = (r+1) * columns + s;
			//printf("	Row: %i | Column: %i | Index: %i\n", r, s, *i);
            *i++ = (r+1) * columns + (s+1);
			//printf("	Row: %i | Column: %i | Index: %i\n", r, s, *i);
            *i++ = (r) * columns + (s+1);
			//printf("	Row: %i | Column: %i | Index: %i\n", r, s, *i);
			//printf("\n");
			//triangle_number++;
		}
	}
}

void Mesh::BuildPrimitive(float radius, unsigned int columns, unsigned int rows) {
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
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(vec3) * 3));	// Same
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

	if (!this->texture_shader.Initialize("texture_shader.vert", "texture_shader.frag"))
		return false;

	this->shaders.push_back(&this->shader);
	this->shaders.push_back(&this->solid_color);
	this->shaders.push_back(&this->stripes_model_space);
	this->shaders.push_back(&this->texture_shader);

	if (this->GLReturnedError("Background::Initialize - on exit"))
		return false;

	return true;
}

void Mesh::TakeDown()
{
	this->vertices.clear();
	this->shader.TakeDown();
	this->solid_color.TakeDown();
	this->texture_shader.TakeDown();
	super::TakeDown();
}

void Mesh::Draw(const ivec2 & size)
{
	assert(false);
}

void Mesh::SetLightPosition(vec3 & light_position)
{
	this->light_position = light_position;
}

void Mesh::Draw(const mat4 & projection, mat4 view, const ivec2 & size, const float time)
{
	if (this->GLReturnedError("Mesh::Draw - on entry"))
		return;

	if(texture.il_handle !=  BAD_IL_VALUE) {
		texture.Bind();
	}

	glEnable(GL_DEPTH_TEST);
	mat4 model = glm::mat4(1.0f);
	//model = scale(model, vec3(10, 10, 10));
	mat4 mvp = projection * view * model;
	mat3 nm = inverse(transpose(mat3(view)));

	this->shaders[this->shader_index]->Use();
	this->GLReturnedError("Mesh::Draw - after use");
	this->shaders[this->shader_index]->CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(view), value_ptr(mvp), value_ptr(nm));

	vec3 light_pos = vec3(0.0f, 0.0f, -20.0f);
	light_pos = nm * light_pos;
	if(shader_index == 3)
		this->texture_shader.CustomSetup(light_pos);

	this->GLReturnedError("Mesh::Draw - after common setup");
	glBindVertexArray(this->vertex_array_handle);
	glDrawElements(GL_TRIANGLES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
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
#include "RenderTexture.h"

RenderTexture::RenderTexture() {
}

RenderTexture::~RenderTexture() {
}

bool RenderTexture::Initialize() {
	this->BuildMesh(100, 100);
	super::Initialize(10.0f);
	return true;
}

void RenderTexture::Draw(const mat4 & projection, mat4 view, const ivec2 & size, Lights & lights,  const float time)
{
	if (this->GLReturnedError("Mesh::Draw - on entry"))
		return;

	/*if(texture.il_handle !=  BAD_IL_VALUE) {
		printf("Binding texture\n");
		texture.Bind(3); //pass this sampler # into texture shaders
	}*/

	glEnable(GL_DEPTH_TEST);
	mat4 model = glm::mat4(1.0f);
	//model = scale(model, vec3(10, 10, 10));
	mat4 mvp = projection * view * model;
	mat3 nm = inverse(transpose(mat3(view)));

	this->shaders[4]->Use();
	this->GLReturnedError("Mesh::Draw - after use");
	this->shaders[4]->CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(view), value_ptr(mvp), value_ptr(nm));

	this->render_texture.CustomSetup(2, lights.GetPosition(0), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);

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
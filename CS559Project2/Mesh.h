#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "object.h"
#include "Shader.h"

using namespace glm;

class Mesh :
	public Object
{
public:
	Mesh(void);
	~Mesh(void);
	bool Initialize(float size);
	virtual void Draw(const glm::ivec2 & size);
	virtual void Draw(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void CalculateSphereNormals(unsigned int sectors, unsigned int rings);
	void StepShader();
	void TakeDown();
	void NormalsHelper(int index, int sectors);
	vec3 GetIndexLeft(int index, int columns, int r, int c);
	vec3 GetIndexRight(int index, int columns, int r, int c);
	vec3 GetIndexUp(int index, int columns, int r, int c);
	vec3 GetIndexDown(int index, int columns, int r, int c, int rows);
	vec3 GetIndexUpLeft(int index, int columns, int r, int c);
	vec3 GetIndexRightUp(int index, int columns, int r, int c);
	vec3 GetIndexDownLeft(int index, int columns, int r, int c, int rows);
	vec3 GetIndexDownRight(int index, int columns, int r, int c, int rows);
	Shader shader;
	Shader solid_color;
	Shader stripes_model_space;
	std::vector<Shader *> shaders;

protected:
	void BuildMesh(unsigned int columns, unsigned int rows);
	void BuildPrimitive(float radius, unsigned int columns, unsigned int rows);
	void BuildNormalVisualizationGeometry();

	glm::vec4 colors[2];
	std::vector<VertexAttributesPCNT> vertices;
	typedef Object super;
	int shader_index;
};


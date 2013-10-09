#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "object.h"
#include "Shader.h"
#include "Mesh.h"
class Ship :
	public Object
{
public:
	Ship(void);
	~Ship(void);
	bool Initialize(float size);
	virtual void Draw(const glm::ivec2 & size);
	virtual void Draw(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void StepShader();
	void TakeDown();
	Shader shader;
	Shader solid_color;
	Shader stripes_model_space;
	std::vector<Shader *> shaders;

private:
	void BuildNormalVisualizationGeometry();
	glm::vec4 colors[2];
	std::vector<VertexAttributesPCNT> vertices;
	typedef Object super;
	int shader_index;
	Mesh mesh;
};


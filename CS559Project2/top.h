/*	Perry Kivolowitz - University of Wisconsin - Madison 
	Computer Sciences Department

	A sample hello world like program demonstrating modern
	OpenGL techniques. 

	Created:	2/25/13
	Updates:
*/

#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "object.h"
#include "shader.h"

class Top : public Object
{
public:
	Top();
	bool Initialize(int slices);
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
	std::vector<VertexAttributesPCN> vertices;
	typedef Object super;
	int shader_index;
};

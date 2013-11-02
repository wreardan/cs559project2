#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Mesh.h"

class RenderTexture : public Mesh
{
public:
	RenderTexture();
	~RenderTexture(void);
	bool Initialize();
	void Draw(int texture_id, const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, Lights & lights, const float time = 0);
	int curr_shader_index;
	void StepPostEffect();

protected:
	typedef Mesh super;
};
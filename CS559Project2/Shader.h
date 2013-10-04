#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <iostream>
class Shader
{
public:
	Shader();
	~Shader(void);
	void TakeDown();
	void Activate();
	void Deactivate();
	bool Initialize(char * vertex_shader_file, char * fragment_shader_file);

	GLuint modelview_matrix_handle;
	GLuint projection_matrix_handle;

	GLuint vertex_shader_id;
	GLuint fragment_shader_id;
	GLuint program_id;
	bool LoadShader(const char * file_name, GLuint shader_id);
	std::stringstream GetShaderLog(GLuint shader_id);
};


#include "Shader.h"
#include <assert.h>
#include "Error.h"
#include <sstream>

using namespace std;

Shader::Shader()
{
	this->vertex_shader_id = (GLuint) -1;
	this->fragment_shader_id = (GLuint) -1;
	this->program_id = (GLuint) -1;
	this->modelview_matrix_handle = (GLuint) -1;
	this->projection_matrix_handle = (GLuint) -1;
}

void Shader::Activate()
{
	assert(this->program_id != (GLuint) -1);
	glUseProgram(this->program_id);
}

void Shader::Deactivate()
{
	assert(this->program_id != (GLuint) -1);
	glUseProgram(0);
}

bool Shader::Initialize(char * vertex_shader_file, char * fragment_shader_file)
{
	GLint check_value;

	if (CheckGLErrors("Shader::Initialize()"))
		return false;

	this->vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	this->LoadShader(vertex_shader_file, this->vertex_shader_id);
	glCompileShader(this->vertex_shader_id);
	glGetShaderiv(this->vertex_shader_id, GL_COMPILE_STATUS, &check_value);
	if (check_value != GL_TRUE)
	{
		cerr << this->GetShaderLog(vertex_shader_id).str();
		cerr << "GLSL compilation failed - vertex shader: " << vertex_shader_file << endl;
		return false;
	}

	if (CheckGLErrors("Shader::Initialize() middle"))
		return false;

	this->fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	this->LoadShader(fragment_shader_file, this->fragment_shader_id);
	glCompileShader(this->fragment_shader_id);
	glGetShaderiv(this->fragment_shader_id, GL_COMPILE_STATUS, &check_value);
	if (check_value != GL_TRUE)
	{
		cerr << this->GetShaderLog(fragment_shader_id).str();
		cerr << "GLSL compilation failed - fragment shader: " << fragment_shader_file << endl;
		return false;
	}

	this->program_id = glCreateProgram();
	glAttachShader(this->program_id, this->vertex_shader_id);
	glAttachShader(this->program_id, this->fragment_shader_id);
	glLinkProgram(program_id);
	GLint link_status;
	glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);
	if(!link_status) {
		char buf[256];
		glGetProgramInfoLog(program_id,256,NULL,buf);
		cout << "error linking: " << buf << endl;
	}

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);
	
	glUseProgram(this->program_id);
	CheckGLErrors("Shader::Initialize() glUseProgram");
//	this->modelview_matrix_handle = glGetUniformLocation(program_id, (const GLchar *) "modelview_matrix");
//	this->projection_matrix_handle = glGetUniformLocation(program_id, (const GLchar *) "projection_matrix");
	glUseProgram(0);

	return !CheckGLErrors("Shader::Initialize() return");
}

void Shader::TakeDown()
{
	GLint temp;
	GLsizei size;

	if (this->program_id == (GLuint) -1)
		return;

	glGetProgramiv(this->program_id, GL_ATTACHED_SHADERS, &temp);
	if (temp > 0)
	{
		GLuint * shader_list = new GLuint[temp];
		glGetAttachedShaders(this->program_id, temp, &size, shader_list);
		for (GLsizei i = 0; i < size; i++)
		{
			glDetachShader(this->program_id, shader_list[i]);
			// The shaders were marked for deletion
			// immediately after they were created.
		}
		delete [] shader_list;
	}
	glDeleteProgram(this->program_id);
	this->program_id = (GLuint) -1;
}

// This function is adapted from OpenGL 4.0 Shading Language Cookbook
// by David Wolff.
bool Shader::LoadShader(const char * file_name, GLuint shader_id)
{
	assert(file_name != NULL);
	if (CheckGLErrors("Shader::LoadShader() start"))
		return false;

	FILE * file_handle = NULL;
	fopen_s(&file_handle, file_name, "rb");
	if (file_handle == NULL)
	{
		cerr <<  "Cannot open shader: " << file_name << endl;
		return false;
	}
	fseek(file_handle, 0, SEEK_END);
	int length = ftell(file_handle);
	fseek(file_handle, 0, SEEK_SET);
	GLubyte * buffer = new GLubyte[length + 1];
	fread(buffer, sizeof(GLubyte), length, file_handle);
	fclose(file_handle);
	buffer[length] = 0;
	glShaderSource(shader_id, 1, (const char **) &buffer, NULL);
	delete [] buffer;

	return !CheckGLErrors("Shader::LoadShader() end");
}

// This function is adapted from OpenGL 4.0 Shading Language Cookbook
// by David Wolff.
stringstream Shader::GetShaderLog(GLuint shader_id)
{
	stringstream s;
	GLint log_length;
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length <= 0)
		s << "No shader log information available." << endl;
	else
	{
		GLchar * buffer = new GLchar[log_length];
		glGetShaderInfoLog(shader_id, log_length, NULL, buffer);
		s << "Shader log:" << endl;
		s << buffer << endl;
		delete [] buffer;
	}
	return s;
}


Shader::~Shader(void)
{
}

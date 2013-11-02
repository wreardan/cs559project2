/*	Perry Kivolowitz - University of Wisconsin - Madison 
	Computer Sciences Department

	A sample hello world like program demonstrating modern
	OpenGL techniques. 

	Created:	2/25/13
	Updates:
*/

#pragma once
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Light.h"

class Shader
{
public:
	Shader();
	void TakeDown();
	virtual void Use();
	virtual bool Initialize(char * vertex_shader_file, char * fragment_shader_file);
	virtual void CustomSetup();
	virtual void CommonSetup(const float time, const GLint * size, const GLfloat * projection, const GLfloat * modelview, const GLfloat * mvp, const GLfloat * nm);

	GLuint modelview_matrix_handle;
	GLuint projection_matrix_handle;
	GLuint normal_matrix_handle;
	GLuint mvp_handle;
	GLuint size_handle;
	GLuint time_handle;

	GLuint vertex_shader_id;
	GLuint fragment_shader_id;
	GLuint program_id;
	bool LoadShader(const char * file_name, GLuint shader_id);
	std::stringstream GetShaderLog(GLuint shader_id);

protected:
	bool GLReturnedError(char * s);
};

class RenderTextureShader : public Shader
{
public: 
	RenderTextureShader();
	bool Initialize(char * vertex_shader_file, char * fragment_shader_file);
	void CustomSetup(GLuint render_text, glm::vec4 light_pos, glm::vec3 light_intensity, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shininess);
protected:
	GLuint render_texture_handle;
	GLuint light_pos_handle;
	GLuint light_intensity_handle;
	GLuint amb_handle;
	GLuint diff_handle;
	GLuint spec_handle;
	GLuint shininess_handle;
private:
	typedef Shader super;
};

class BackgroundShader : public Shader
{
public:
	BackgroundShader();
	virtual bool Initialize(char * vertex_shader_file, char * fragment_shader_file);
	virtual void CustomSetup(glm::vec4 * color_array);

protected:
	GLuint color_array_handle;

private:
	typedef Shader super;
};


class TextureShader : public Shader
{
public:
	TextureShader();
	virtual bool Initialize(char * vertex_shader_file, char * fragment_shader_file);
	virtual void CustomSetup(GLuint text, glm::vec4 & light_position);

protected:
	GLuint texture_sampler;
	GLuint light_position_handle;
	glm::vec4 light_position;

private:
	typedef Shader super;
};

#include "glslprogram.h"

class SpotlightShader : public Shader
{
public:
	SpotlightShader();
	virtual bool Initialize(char * vertex_shader_file, char * fragment_shader_file);
	virtual void CustomSetup(int texture_id, Lights & lights);

protected:
	GLuint texture_sampler;
	GLuint light_position_handle;

	GLuint spotlight_intensity_handle;
	GLuint spotlight_exponent_handle;
	GLuint spotlight_cutoff_handle;

	GLuint spotlight_position_handle;
	GLuint spotlight_direction_handle;
	
	GLuint kd_handle;
	GLuint ks_handle;
	GLuint ka_handle;
	GLuint shininess_handle;

private:
	typedef Shader super;
};

class SpotlightWireframeShader : public Shader
{
public:
	SpotlightWireframeShader();
	bool Initialize(char * vertex_shader_file, char * fragment_shader_file, char * geometry_shader_file);
	virtual bool Initialize(char * vertex_shader_file, char * fragment_shader_file);
	virtual void CommonSetup(const float time, const GLint * size, const GLfloat * projection, const GLfloat * modelview, const GLfloat * mvp, const GLfloat * nm);
	virtual void CustomSetup(int texture_id, const float time, const glm::ivec2 & size, const glm::mat4 & projection, const glm::mat4 & modelview,
		const glm::mat4 & mvp, const glm::mat3 & normal_matrix, Lights & lights);
	void TakeDown();
	virtual void Use();

protected:
	GLSLProgram prog;

private:
	typedef Shader super;
};

/*	Perry Kivolowitz - University of Wisconsin - Madison 
	Computer Sciences Department

	A sample hello world like program demonstrating modern
	OpenGL techniques. 

	Created:	2/25/13
	Updates:
*/

#include "shader.h"
#include <assert.h>

using namespace std;
using namespace glm;

#define BAD_GL_VALUE GLuint(-1)

Shader::Shader()
{
	this->vertex_shader_id = BAD_GL_VALUE;
	this->fragment_shader_id = BAD_GL_VALUE;
	this->program_id = BAD_GL_VALUE;
	this->modelview_matrix_handle = BAD_GL_VALUE;
	this->projection_matrix_handle = BAD_GL_VALUE;
	this->normal_matrix_handle = BAD_GL_VALUE;
	this->size_handle = BAD_GL_VALUE;
}

/*	This Shader() class implements or assumes a basic set of uniforms will be
	provided to all shaders derived from it. These are listed below. 
	Shader::CommonSetup() can be used by call derived classes to send the 
	common values to the shader. Values unique to the derived class can be
	loaded with the CustomShader() function.
*/

void Shader::CommonSetup(const float time, const GLint * size, const GLfloat * projection, const GLfloat * modelview, const GLfloat * mvp, const GLfloat * nm)
{
	if (this->time_handle != BAD_GL_VALUE)
		glUniform1f(this->time_handle, time);
	this->GLReturnedError("Top::Draw - after time_handle");
	if (this->size_handle != BAD_GL_VALUE)
		glUniform2iv(this->size_handle, 1, size);
	this->GLReturnedError("Top::Draw - after size_handle");
	if (this->projection_matrix_handle != BAD_GL_VALUE)
		glUniformMatrix4fv(this->projection_matrix_handle, 1, GL_FALSE, projection);
	this->GLReturnedError("Top::Draw - after projection_matrix_handle");
	if (this->modelview_matrix_handle != BAD_GL_VALUE)
		glUniformMatrix4fv(this->modelview_matrix_handle, 1, GL_FALSE, modelview);
	this->GLReturnedError("Top::Draw - after modelview_matrix_handle");
	if (this->mvp_handle != BAD_GL_VALUE)
		glUniformMatrix4fv(this->mvp_handle, 1, GL_FALSE, mvp);
	this->GLReturnedError("Top::Draw - after mvp_handle");
	if (this->normal_matrix_handle != BAD_GL_VALUE)
		glUniformMatrix3fv(this->normal_matrix_handle, 1, GL_FALSE, nm);
	this->GLReturnedError("Top::Draw - after normal_matrix_handle");
}

void Shader::Use()
{
	assert(this->program_id != BAD_GL_VALUE);
	glUseProgram(this->program_id);
}

/*	The shader initialization code is lifted liberally from the GLSL 4.0 Cookbook.
*/

bool Shader::Initialize(char * vertex_shader_file, char * fragment_shader_file)
{
	GLint check_value;

	if (GLReturnedError("Shader::Initialize - on entrance"))
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

	if (GLReturnedError("Shader::Initialize - after processing vertex shader"))
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

	//Check for GL Shader Linking Errors
	GLint link_status;
	glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);
	if(!link_status) {
		char buf[4096];
		glGetProgramInfoLog(program_id,4096,NULL,buf);
		cout << "Shader::Initialize() error linking shader: " << buf << endl;
		return false;
	}


	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	glUseProgram(this->program_id);

	this->modelview_matrix_handle = glGetUniformLocation(this->program_id, (const GLchar *) "modelview_matrix");
	this->projection_matrix_handle = glGetUniformLocation(this->program_id, (const GLchar *) "projection_matrix");
	this->normal_matrix_handle = glGetUniformLocation(this->program_id, (const GLchar *) "normal_matrix");
	this->mvp_handle = glGetUniformLocation(this->program_id, (const GLchar *) "mvp");
	this->size_handle = glGetUniformLocation(this->program_id, (const GLchar *) "size");
	this->time_handle = glGetUniformLocation(this->program_id, (const GLchar *) "time");

	glUseProgram(0);

	return !GLReturnedError("Shader::Initialize - on exit");
}

void Shader::CustomSetup()
{
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

/*
	This function is adapted from OpenGL 4.0 Shading Language Cookbook by David Wolff.
*/

bool Shader::LoadShader(const char * file_name, GLuint shader_id)
{
	assert(file_name != NULL);
	if (GLReturnedError("Shader::LoadShader - on entrance"))
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

	return !GLReturnedError("Shader::LoadShader - on exit");
}

/*
	This function is adapted from OpenGL 4.0 Shading Language Cookbook by David Wolff.
*/

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

bool Shader::GLReturnedError(char * s)
{
	bool return_error = false;
	GLenum glerror;

	while ((glerror = glGetError()) != GL_NO_ERROR)
	{
		return_error = true;
		cerr << s << ": " << gluErrorString(glerror) << endl;
	}

	return return_error;
}

RenderTextureShader::RenderTextureShader() : super()
{
	this->render_texture_handle = BAD_GL_VALUE;
	this->amb_handle = BAD_GL_VALUE;
	this->diff_handle = BAD_GL_VALUE;
	this->spec_handle = BAD_GL_VALUE;
	this->light_pos_handle = BAD_GL_VALUE;
	this->shininess_handle = BAD_GL_VALUE;
}

bool RenderTextureShader::Initialize(char * vertex_shader_file, char * fragment_shader_file) {
	if (!super::Initialize(vertex_shader_file, fragment_shader_file))
		return false;

	this->Use();
	this->render_texture_handle = glGetUniformLocation(this->program_id, (const GLchar *) "RenderTex");
	this->amb_handle = glGetUniformLocation(this->program_id, (const GLchar *) "Material.Ka");
	this->diff_handle = glGetUniformLocation(this->program_id, (const GLchar *) "Material.Kd");
	this->spec_handle = glGetUniformLocation(this->program_id, (const GLchar *) "Material.Ks");
	this->light_pos_handle = glGetUniformLocation(this->program_id, (const GLchar *) "Light.Position");
	this->light_intensity_handle = glGetUniformLocation(this->program_id, (const GLchar *) "Light.Intensity");
	this->shininess_handle = glGetUniformLocation(this->program_id, (const GLchar *) "Material.Shininess");
	
	this->GLReturnedError("RenderTextureShader::Initialize");

	glUseProgram(0);
	return true;
}

void RenderTextureShader::CustomSetup(GLuint render_text, glm::vec4 light_pos, glm::vec3 light_intensity, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shininess) {
	glUniform1i(this->render_texture_handle, render_text);
//	glUniform4f(this->program_id, light_pos.x, light_pos.y, light_pos.z, light_pos.w);
//	glUniform3f(this->program_id, light_intensity.x, light_intensity.y, light_intensity.z);
//	glUniform3f(this->program_id, amb.x, amb.y, amb.z);
//	glUniform3f(this->program_id, diff.x, diff.y, diff.z);
//	glUniform3f(this->program_id, spec.x, spec.y, spec.z);
//	glUniform1f(this->program_id, shininess);
	this->GLReturnedError("RenderTextureShader::CustomSetup");
}

BackgroundShader::BackgroundShader() : super()
{
	this->color_array_handle = BAD_GL_VALUE;
}

bool BackgroundShader::Initialize(char * vertex_shader_file, char * fragment_shader_file)
{
	if (!super::Initialize(vertex_shader_file, fragment_shader_file))
		return false;

	this->Use();
	this->color_array_handle = glGetUniformLocation(this->program_id, (const GLchar *) "color_array");
	glUseProgram(0);
//	assert(this->color_array_handle != BAD_GL_VALUE);
	return true;
}

void BackgroundShader::CustomSetup(vec4 * color_array)
{
	glUniform4fv(this->color_array_handle, 4, (GLfloat *) color_array);
}


TextureShader::TextureShader() : super()
{
	this->texture_sampler = BAD_GL_VALUE;
}

bool TextureShader::Initialize(char * vertex_shader_file, char * fragment_shader_file)
{
	if (!super::Initialize(vertex_shader_file, fragment_shader_file))
		return false;

	this->Use();
	this->texture_sampler = glGetUniformLocation(this->program_id, (const GLchar *) "s_texture");
	this->light_position_handle = glGetUniformLocation(this->program_id, (const GLchar *) "light_position");
	this->GLReturnedError("TextureShader::Initialize - after light_position_handle");
	glUseProgram(0);
//	assert(this->color_array_handle != BAD_GL_VALUE);
	return true;
}

void TextureShader::CustomSetup(GLuint text, vec4 & light_position)
{
	this->light_position = light_position;
	glUniform1i(this->texture_sampler, text);
	glUniform3fv(this->light_position_handle, 1, value_ptr(light_position));
	this->GLReturnedError("TextureShader::CustomSetup - after light_position");
}


SpotlightShader::SpotlightShader() : super()
{
	this->texture_sampler = BAD_GL_VALUE;
}

bool SpotlightShader::Initialize(char * vertex_shader_file, char * fragment_shader_file)
{
	if (!super::Initialize(vertex_shader_file, fragment_shader_file))
		return false;

	this->Use();
	this->texture_sampler = glGetUniformLocation(this->program_id, (const GLchar *) "s_texture");

	spotlight_intensity_handle = glGetUniformLocation(this->program_id, (const GLchar *) "Spot.intesity");
	glUniform3fv(this->spotlight_intensity_handle, 1, value_ptr(vec3(0.9f,0.9f,0.9f)));

	spotlight_exponent_handle = glGetUniformLocation(this->program_id, (const GLchar *) "Spot.exponent");
	glUniform1f(this->spotlight_exponent_handle, 30.0f);

	spotlight_cutoff_handle = glGetUniformLocation(this->program_id, (const GLchar *) "Spot.cutoff");
	glUniform1f(this->spotlight_cutoff_handle, 5.0f);

	kd_handle = glGetUniformLocation(this->program_id, (const GLchar *) "Kd");
	glUniform3fv(this->kd_handle, 1, value_ptr(vec3(0.9f, 0.5f, 0.3f)));
	ks_handle = glGetUniformLocation(this->program_id, (const GLchar *) "Ks");
	glUniform3fv(this->ks_handle, 1, value_ptr(vec3(0.95f, 0.95f, 0.95f)));
	ka_handle = glGetUniformLocation(this->program_id, (const GLchar *) "Ka");
	glUniform3fv(this->ka_handle, 1, value_ptr(vec3(0.9f * 0.3f, 0.5f * 0.3f, 0.3f * 0.3f)));
	shininess_handle = glGetUniformLocation(this->program_id, (const GLchar *) "Shininess");
	glUniform1f(this->shininess_handle, 100.0f);

	spotlight_position_handle = glGetUniformLocation(this->program_id, (const GLchar *) "Spot.position");
	spotlight_direction_handle = glGetUniformLocation(this->program_id, (const GLchar *) "Spot.direction");
	light_position_handle = glGetUniformLocation(this->program_id, (const GLchar *) "light_position");

	this->GLReturnedError("SpotlightShader::Initialize - after light_position_handle");
	glUseProgram(0);
//	assert(this->color_array_handle != BAD_GL_VALUE);
	return true;
}

void SpotlightShader::CustomSetup(int texture_id, Lights & lights)
{
	glUniform4fv(this->spotlight_position_handle, 1, value_ptr(lights.GetRawPosition(1)));
	glUniform3fv(this->spotlight_direction_handle, 1, value_ptr(lights.GetRawDirection(1)));
	glUniform3fv(this->light_position_handle, 1, value_ptr(lights.GetPosition(0)));
	glUniform1i(this->texture_sampler, texture_id);

	this->GLReturnedError("SpotlightShader::CustomSetup - after spotlight_position_handle,spotlight_direction_handle");
}


SpotlightWireframeShader::SpotlightWireframeShader()
{

}

bool SpotlightWireframeShader::Initialize(char * vertex_shader_file, char * fragment_shader_file, char * geometry_shader_file)
{
	try {
		prog.compileShader(vertex_shader_file, GLSLShader::VERTEX);
		prog.compileShader(fragment_shader_file, GLSLShader::FRAGMENT);
		prog.compileShader(geometry_shader_file, GLSLShader::GEOMETRY);
		prog.link();
		prog.validate();
		prog.use();
	} catch ( GLSLProgramException & exception) {
		cerr << "SpotlightWireframeShader::Initialize() error " << exception.what() << endl;
	} catch (...) {
		cerr << "SpotlightWireframeShader::Initialize() other error " << endl;
	}

	return true;
}

bool SpotlightWireframeShader::Initialize(char * vertex_shader_file, char * fragment_shader_file)
{
	assert(false);
	return false;
}

void SpotlightWireframeShader::CommonSetup(const float time, const GLint * size, const GLfloat * projection, const GLfloat * modelview, const GLfloat * mvp, const GLfloat * nm)
{
	//do nothing
}

void SpotlightWireframeShader::CustomSetup(int texture_id, const float time, const glm::ivec2 & size, const glm::mat4 & projection, const glm::mat4 & modelview,
		const glm::mat4 & mvp, const glm::mat3 & normal_matrix, Lights & lights, int wireframe_mode)
{
	prog.use();

	prog.setUniform("modelview_matrix", modelview);
	prog.setUniform("normal_matrix", normal_matrix);
	prog.setUniform("mvp", mvp);	//Found

	float w2 = size.x / 2.0f;
	float h2 = size.y / 2.0f;
	mat4 viewport_matrix = mat4( vec4(w2,0.0f,0.0f,0.0f),
		vec4(0.0f,h2,0.0f,0.0f),
		vec4(0.0f,0.0f,1.0f,0.0f),
		vec4(w2+0, h2+0, 0.0f, 1.0f));
	prog.setUniform("ViewportMatrix", viewport_matrix);

	prog.setUniform("Kd", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Ks", 0.95f, 0.95f, 0.95f);
    //prog.setUniform("Ka", 0.9f * 0.3f, 0.5f * 0.3f, 0.3f * 0.3f);
    prog.setUniform("Ka", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Shininess", 100.0f);

	prog.setUniform("s_texture", texture_id);	//Found
	
	switch(wireframe_mode)
	{
	case 1:
		prog.setUniform("Line.Width", 0.5f);
		prog.setUniform("Line.Color", vec4(0.0f,0.0f,1.0f,1.0f));
		break;
	case 2:
		prog.setUniform("Line.Width", 2.0f);
		prog.setUniform("Line.Color", vec4(0.5f,0.0f,0.5f,1.0f));
		break;
	case 3:
		prog.setUniform("Line.Width", 0.2f);
		prog.setUniform("Line.Color", vec4(0.2f,0.2f,0.2f,1.0f));
		break;
	case 0:
	default:
		prog.setUniform("Line.Width", -1.0f);
	prog.setUniform("Line.Color", vec4(0.0f,0.0f,1.0f,1.0f));
	}

	prog.setUniform("Spot.intensity", vec3(1.0f,1.0f,1.0f));

	prog.setUniform("Spot.exponent", 1000.0f);

	prog.setUniform("Spot.cutoff", 60.0f);	//Found
	
	prog.setUniform("Spot.position", lights.GetRawPosition(1));	//Found
	prog.setUniform("Spot.direction", lights.GetRawDirection(1));	//Found
	prog.setUniform("light_position", vec3(lights.GetPosition(0)));	//Found
}

void SpotlightWireframeShader::TakeDown()
{
	
}


void SpotlightWireframeShader::Use()
{
	prog.use();
}

bool SpotlightWireframeShadowsShader::Initialize(char * vertex_shader_file, char * fragment_shader_file, char * geometry_shader_file)
{
	super::Initialize(vertex_shader_file, fragment_shader_file, geometry_shader_file);

	pass1Index = glGetSubroutineIndex( prog.getHandle(), GL_FRAGMENT_SHADER, "recordDepth");
    pass2Index = glGetSubroutineIndex( prog.getHandle(), GL_FRAGMENT_SHADER, "shadeWithShadow");

	return true;
}

void SpotlightWireframeShadowsShader::CustomSetup(int texture_id, int shadow_texture_id, const float time, const glm::ivec2 & size, const glm::mat4 & projection, const glm::mat4 & modelview,
	const glm::mat4 & mvp, const glm::mat3 & normal_matrix, Lights & lights, int wireframe_mode, glm::mat4 shadow_matrix, int shadow_pass_type)
{
	super::CustomSetup(texture_id, time, size, projection, modelview, mvp, normal_matrix, lights, wireframe_mode);

	prog.setUniform("ShadowMatrix", shadow_matrix);
	prog.setUniform("shadow_map_texture", shadow_texture_id);

	if(shadow_pass_type == 0)
		shadow_pass_type = pass1Index;
	else
		shadow_pass_type = pass2Index;

	glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &pass1Index);
	//prog.setUniform("RenderPassType", shadow_pass_type);
}

#version 400

/*	Perry Kivolowitz - UW - Madison - CS 559 Demo
	Background Shader
*/

layout (location = 0) in vec3 vertex_position;

uniform mat4 mvp;

/*	Strictly speaking, even multiplication by the mvp can be avoided.
*/

void main()
{
	gl_Position = mvp * vec4(vertex_position, 1.0);
}

/*	Samples from four  different epochs in OpenGL development
	in the same program. 

	Perry Kivolowitz
	Computer Sciences Department - University of Wisconsin.
*/

#version 400

layout (location = 0) in vec2 VertexPosition;

void main()
{
	gl_Position = vec4(VertexPosition, 0, 1);
}
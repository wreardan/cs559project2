#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 2) in vec3 vertex_color;

uniform mat4 mvp;

flat out vec3 color;

void main()
{
	//http://stackoverflow.com/questions/12964279/whats-the-origin-of-this-glsl-rand-one-liner
	gl_PointSize = fract(sin(dot(vertex_position.xy, vec2(12.9898,78.233))) * 43758.5453) * 4;
	gl_Position = mvp * vec4(vertex_position, 1.0);
	color = vertex_color;
}

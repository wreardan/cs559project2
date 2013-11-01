/*
Spotlight Wireframe Fragment Shader
Code inspired by
https://github.com/daw42/glslcookbook/blob/master/chapter06/shader/shadewire.fs
*/

#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;

layout (location = 2) in vec3 vertex_color;
layout (location = 3) in vec2 vertex_texture;

uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;
uniform mat4 mvp;

flat out vec3 color;
out vec3 position;
out vec3 normal;
out vec2 texture;

void main()
{
	color = vertex_color;
    normal = normalize( normal_matrix * vertex_normal);
    position = vec3( modelview_matrix * vec4(vertex_position,1.0) );
	texture = vertex_texture;

    gl_Position = mvp * vec4(vertex_position,1.0);
}
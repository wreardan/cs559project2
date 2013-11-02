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

//Outputs to Geometry Shader
flat out vec3 Vcolor;
out vec3 Vposition;
out vec3 Vnormal;
out vec2 Vtexture;

void main()
{
	Vcolor = vertex_color;
    Vnormal = normalize( normal_matrix * vertex_normal);
    Vposition = vec3( modelview_matrix * vec4(vertex_position,1.0) );
	Vtexture = vertex_texture;

    gl_Position = mvp * vec4(vertex_position,1.0);
}
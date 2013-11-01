/*
Spotlight Wireframe Geometry Shader
Code inspired by
https://github.com/daw42/glslcookbook/blob/master/chapter06/shader/shadewire.gs
*/

#version 400

layout( triangles ) in;
layout( triangle_strip, max_vertices = 3 ) out;

out vec3 normal;
out vec3 position;
flat out vec3 color;
out vec2 texture;
noperspective out vec3 GEdgeDistance;

in vec3 normal_in[];
in vec3 position_in[];
in vec2 texture_in[];
flat in vec3 color_in[];

uniform mat4 ViewportMatrix;  // Viewport matrix

void main()
{
    // Transform each vertex into viewport space
    vec2 p0 = vec2(ViewportMatrix * (gl_in[0].gl_Position / gl_in[0].gl_Position.w));
    vec2 p1 = vec2(ViewportMatrix * (gl_in[1].gl_Position / gl_in[1].gl_Position.w));
    vec2 p2 = vec2(ViewportMatrix * (gl_in[2].gl_Position / gl_in[2].gl_Position.w));

    float a = length(p1 - p2);
    float b = length(p2 - p0);
    float c = length(p1 - p0);
    float alpha = acos( (b*b + c*c - a*a) / (2.0*b*c) );
    float beta = acos( (a*a + c*c - b*b) / (2.0*a*c) );
    float ha = abs( c * sin( beta ) );
    float hb = abs( c * sin( alpha ) );
    float hc = abs( b * sin( alpha ) );

    GEdgeDistance = vec3( ha, 0, 0 );
    normal = normal_in[0];
    position = position_in[0];
	color = color_in[0];
	texture = texture_in[0];
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    GEdgeDistance = vec3( 0, hb, 0 );
    normal = normal_in[1];
    position = position_in[1];
	color = color_in[1];
	texture = texture_in[1];
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    GEdgeDistance = vec3( 0, 0, hc );
    normal = normal_in[2];
    position = position_in[2];
	color = color_in[2];
	texture = texture_in[2];
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();
}
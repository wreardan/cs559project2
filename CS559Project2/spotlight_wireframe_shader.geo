/*
Spotlight Wireframe Geometry Shader
Code liberally lifted from
https://github.com/daw42/glslcookbook/blob/master/chapter06/shader/shadewire.gs
*/

#version 400

layout( triangles ) in;
layout( triangle_strip, max_vertices = 3 ) out;

//Outputs to Fragment Shader
out vec3 Gposition;
out vec3 Gnormal;
flat out vec3 Gcolor;
out vec2 Gtexture;
noperspective out vec3 GEdgeDistance;

//Inputs from Vertex Shader
in vec3 Vposition[];
in vec3 Vnormal[];
flat in vec3 Vcolor[];
in vec2 Vtexture[];

uniform mat4 ViewportMatrix;  // Viewport matrix

void main()
{
    // Transform each vertex into viewport space
    vec2 p0 = vec2(ViewportMatrix * (gl_in[0].gl_Position / gl_in[0].gl_Position.w));
    vec2 p1 = vec2(ViewportMatrix * (gl_in[1].gl_Position / gl_in[1].gl_Position.w));
    vec2 p2 = vec2(ViewportMatrix * (gl_in[2].gl_Position / gl_in[2].gl_Position.w));

	//Compute the Triangle's Altitude (ha, hb, hc) using the law of cosines
    float a = length(p1 - p2);
    float b = length(p2 - p0);
    float c = length(p1 - p0);
    float alpha = acos( (b*b + c*c - a*a) / (2.0*b*c) );
    float beta = acos( (a*a + c*c - b*b) / (2.0*a*c) );
    float ha = abs( c * sin( beta ) );
    float hb = abs( c * sin( alpha ) );
    float hc = abs( b * sin( alpha ) );

	//vertex A
    GEdgeDistance = vec3( ha, 0, 0 );
    Gposition = Vposition[0];
    Gnormal = Vnormal[0];
	Gcolor = Vcolor[0];
	Gtexture = Vtexture[0];
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
	
	//vertex B
    GEdgeDistance = vec3( 0, hb, 0 );
    Gposition = Vposition[1];
    Gnormal = Vnormal[1];
	Gcolor = Vcolor[1];
	Gtexture = Vtexture[1];
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
	
	//vertex C
    GEdgeDistance = vec3( 0, 0, hc );
    Gposition = Vposition[2];
    Gnormal = Vnormal[2];
	Gcolor = Vcolor[2];
	Gtexture = Vtexture[2];
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();
}
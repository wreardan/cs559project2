#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 3) in vec2 VertexTexCoord;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

/*uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;*/

uniform mat4 mvp;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;
uniform mat4 projection_matrix;

void main()
{
    TexCoord = VertexTexCoord;
    Normal = normalize( normal_matrix * VertexNormal);
    Position = vec3( modelview_matrix * vec4(VertexPosition,1.0) );

    gl_Position = mvp * vec4(VertexPosition,1.0);
}
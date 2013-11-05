#version 400

//basic noise to screen effect (used random function sourced in solid_shader)

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D RenderTex;

layout( location = 0 ) out vec4 FragColor;

void main() {
    vec4 texColor = texture( RenderTex, TexCoord);
	texColor = texColor * fract(sin(dot(TexCoord.xy, vec2(12.9898,78.233))) * 43758.5453) * 4;
    FragColor = texColor;
}
#version 400

//toony, posterize post effect adapted from:http://www.geeks3d.com/20091027/shader-library-posterization-post-processing-effect-glsl/

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D RenderTex;

layout( location = 0 ) out vec4 FragColor;

void main() {
    vec4 texColor = texture( RenderTex, TexCoord);
	vec3 c = texColor.xyz;
	c = pow(c, vec3(0.6, 0.6, 0.6));
	c = c * 8;
	c = floor(c);
	c = c / 8;
	c = pow(c, vec3(1.0/0.6));
	texColor = vec4(c.x, c.y, c.z, 1.0);
    FragColor = texColor;
}
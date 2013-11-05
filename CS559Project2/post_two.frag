#version 400

//predator vision post effect adapted from: http://www.geeks3d.com/20101123/shader-library-predators-thermal-vision-post-processing-filter-glsl/

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D RenderTex;

layout( location = 0 ) out vec4 FragColor;

void main() {
    vec4 texColor = texture( RenderTex, TexCoord);
	vec3 tc = vec3(1.0, 1.0, 1.0);

	vec3 c = texColor.xyz;
	vec3 colors[3];
	colors[0] = vec3(0.0, 0.0, 1.0);
	colors[1] = vec3(1.0, 1.0, 0.0);
	colors[2] = vec3(1.0, 0.0, 0.0);
	float lum = dot(vec3(0.30, 0.59, 0.11), c.rgb); //(c.r + c.g + c.b) / 3.0;
	int ix = (lum < 0.5) ? 0:1;
	tc = mix(colors[ix], colors[ix + 1], (lum - float(ix) * 0.5) / 0.5);

    FragColor = vec4(tc, 1.0);
}
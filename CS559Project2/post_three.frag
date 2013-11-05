#version 400

//dream vision post effect [http://www.geeks3d.com/20091112/shader-library-dream-vision-post-processing-filter-glsl/]

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D RenderTex;

layout( location = 0 ) out vec4 FragColor;

void main() {
    vec4 texColor = texture( RenderTex, TexCoord);

	texColor += texture(RenderTex, TexCoord + 0.001);
	texColor += texture(RenderTex, TexCoord + 0.003);
	texColor += texture(RenderTex, TexCoord + 0.005);
	texColor += texture(RenderTex, TexCoord + 0.007);
	texColor += texture(RenderTex, TexCoord + 0.009);
	texColor += texture(RenderTex, TexCoord + 0.011);

	texColor += texture(RenderTex, TexCoord - 0.001);
	texColor += texture(RenderTex, TexCoord - 0.003);
	texColor += texture(RenderTex, TexCoord - 0.005);
	texColor += texture(RenderTex, TexCoord - 0.007);
	texColor += texture(RenderTex, TexCoord - 0.009);
	texColor += texture(RenderTex, TexCoord - 0.011);

	texColor.rgb = vec3((texColor.r + texColor.g + texColor.b) / 3.0);
	texColor = texColor / 9.5;
	
    FragColor = texColor;
}
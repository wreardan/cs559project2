#version 400

//scan lines and faked chromatic aberration 
//inpiration/methods used from: http://blog.jahfer.com/2012/04/02/experimenting-shaders-openframeworks/, 
//http://cpansearch.perl.org/src/CORION/App-VideoMixer-0.02/filters/scanlines.glsl

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D RenderTex;

layout( location = 0 ) out vec4 FragColor;

void main() {
    float global_pos = (TexCoord.y + 0.2) * 300.0;
    float wave_pos = cos((fract( global_pos ) - 0.5)*3.14);
    vec4 texColor = texture2D(RenderTex, TexCoord );

	texColor.r = texture2D(RenderTex, TexCoord).r;
    texColor.g = texture2D(RenderTex, TexCoord + 0.004).g;
    texColor.b = texture2D(RenderTex, TexCoord - 0.004).b;

    FragColor = mix(vec4(0,0,0,0), texColor, wave_pos);
}
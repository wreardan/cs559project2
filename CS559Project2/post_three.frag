#version 400

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D RenderTex;

struct LightInfo {
  vec4 Position;  // Light position in eye coords.
  vec3 Intensity; // A,D,S intensity
};
uniform LightInfo Light;

struct MaterialInfo {
  vec3 Ka;            // Ambient reflectivity
  vec3 Kd;            // Diffuse reflectivity
  vec3 Ks;            // Specular reflectivity
  float Shininess;    // Specular shininess factor
};
uniform MaterialInfo Material;

layout( location = 0 ) out vec4 FragColor;

vec3 phongModel( vec3 pos, vec3 norm ) {
    vec3 s = normalize(vec3(Light.Position) - pos);
    vec3 v = normalize(-pos.xyz);
    vec3 r = reflect( -s, norm );
    vec3 ambient = Light.Intensity * Material.Ka;
    float sDotN = max( dot(s,norm), 0.0 );
    vec3 diffuse = Light.Intensity * Material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 )
        spec = Light.Intensity * Material.Ks *
               pow( max( dot(r,v), 0.0 ), Material.Shininess );

    return ambient + diffuse + spec;
}

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
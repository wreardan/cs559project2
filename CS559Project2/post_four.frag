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
    float global_pos = (TexCoord.y + 0.2) * 300.0;
    float wave_pos = cos((fract( global_pos ) - 0.5)*3.14);
    vec4 texColor = texture2D(RenderTex, TexCoord );

	texColor.r = texture2D(RenderTex, TexCoord).r;
    texColor.g = texture2D(RenderTex, TexCoord + 0.004).g;
    texColor.b = texture2D(RenderTex, TexCoord - 0.004).b;

    FragColor = mix(vec4(0,0,0,0), texColor, wave_pos);
}
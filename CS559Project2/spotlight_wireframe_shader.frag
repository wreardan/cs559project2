/*
Code taken from GLSL 4.0 Cookboook for Wire shader, Chapter 6
Also based on Spotlight shader
https://github.com/daw42/glslcookbook/blob/master/chapter06/shader/shadewire.fs
*/

#version 400

in vec3 Gposition;
in vec3 Gnormal;
flat in vec3 Gcolor;
in vec2 Gtexture;
noperspective in vec3 GEdgeDistance;

struct SpotLightInfo {
    vec4 position;   // Gposition in eye coords
    vec3 intensity;  // Amb., Diff., and Specular intensity
    vec3 direction;  // Direction of the spotlight in eye coords.
    float exponent;  // Angular attenuation exponent
    float cutoff;    // Cutoff angle (between 0 and 90)
};
uniform SpotLightInfo Spot;

uniform vec3 light_position;

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ka;            // Ambient reflectivity
uniform vec3 Ks;            // Specular reflectivity
uniform float Shininess;    // Specular shininess factor

uniform struct LineInfo {
	float Width;
	vec4 Color;
} Line;

uniform sampler2D s_texture;

layout( location = 0 ) out vec4 FragColor;

vec3 adsWithSpotlight( )
{
    vec3 s = normalize( vec3( Spot.position) - Gposition );
    vec3 spotDir = normalize( Spot.direction);
    float angle = acos( dot(-s, spotDir) );
    float cutoff = radians( clamp( Spot.cutoff, 0.0, 90.0 ) );
    vec3 ambient = Spot.intensity * Ka;

    if( angle < Spot.cutoff ) {
        float spotFactor = pow( dot(-s, spotDir), Spot.exponent );
        vec3 v = normalize(vec3(-Gposition));
        vec3 h = normalize( v + s );

        return
            ambient +
            spotFactor * Spot.intensity * (
              Kd * max( dot(s, Gnormal), 0.0 ) +
              Ks * pow( max( dot(h,Gnormal), 0.0 ), Shininess )
           );
    } else {
        return ambient;
    }
}

/*
vec3 ads( vec3 pos, vec3 norm ) {
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
*/

vec3 ads( )
{
  vec3 n = Gnormal;

  if (!gl_FrontFacing)
	n = -n;

  vec3 s = normalize(light_position - Gposition);

  vec3 v = normalize(-Gposition);
  vec3 r = reflect(-s, n);
  float s_dot_n = max(dot(s, n), 0.0);

  return Gcolor * s_dot_n + (s_dot_n > 0 ? Gcolor * pow(max(dot(r, v), 0.0), Shininess) : vec3(0.0));
}

void main() {

	// Find the smallest edge distance
    float d = min( GEdgeDistance.x, GEdgeDistance.y );
    d = min( d, GEdgeDistance.z );

    float mixVal;
    if( d < Line.Width - 1 ) {
        mixVal = 1.0;
    } else if( d > Line.Width + 1 ) {
        mixVal = 0.0;
    } else {
        float x = d - (Line.Width - 1);
        mixVal = exp2(-2.0 * (x*x));
    }

	//calculate the color value with ADS, 
    vec4 t_color = texture2D(s_texture, Gtexture);

	vec4 lit_color = vec4(ads(), 1.0) * t_color * vec4(Gcolor, 1.0);
	vec4 lit_color2 = vec4(adsWithSpotlight() * Gcolor, 1.0);

	//FragColor = lit_color + lit_color2;
	FragColor = mix(lit_color + lit_color2, Line.Color, mixVal); 
}
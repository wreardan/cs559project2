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

struct SpotLightInfo {
    vec4 Gposition;   // Gposition in eye coords
    vec3 intensity;
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

//in vec3 GGposition;
//in vec3 GGnormal;
noperspective in vec3 GEdgeDistance;

uniform sampler2D s_texture;

layout( location = 0 ) out vec4 FragGcolor;

vec3 adss( )
{
  vec3 n = Gnormal;

  if (!gl_FrontFacing)
	n = -n;

  vec3 s = normalize(vec3(Spot.Gposition) - Gposition);

    vec3 spotDir = normalize( Spot.direction);
    float angle = acos( dot(-s, spotDir) );
    float cutoff = radians( clamp( Spot.cutoff, 0.0, 90.0 ) );

    if( angle < cutoff ) {

	  vec3 v = normalize(-Gposition);
	  vec3 r = reflect(-s, n);
	  float s_dot_n = max(dot(s, n), 0.0);

	  return Gcolor * s_dot_n + (s_dot_n > 0 ? Gcolor * pow(max(dot(r, v), 0.0), Shininess) : vec3(0.0));
	}
	return vec3(0.0);
}

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
    vec4 t_Gcolor = texture2D(s_texture, Gtexture);
	vec4 lit_Gcolor = vec4(adss() + ads(), 1.0) * t_Gcolor;

	// Find the smallest distance
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

	FragGcolor = mix(lit_Gcolor, Line.Color, mixVal); 
}
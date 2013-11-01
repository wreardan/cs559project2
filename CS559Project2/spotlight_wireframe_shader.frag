/*
Code taken from GLSL 4.0 Cookboook for Spotlight, Chapter 3
https://github.com/daw42/glslcookbook/blob/master/chapter03/shader/spot.fs
*/

#version 400

flat in vec3 color;
in vec3 position;
in vec3 normal;
in vec2 texture_coord;

struct SpotLightInfo {
    vec4 position;   // position in eye coords
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

uniform sampler2D s_texture;

layout( location = 0 ) out vec4 FragColor;

vec3 adss( )
{
  vec3 n = normal;

  if (!gl_FrontFacing)
	n = -n;

  vec3 s = normalize(vec3(Spot.position) - position);

    vec3 spotDir = normalize( Spot.direction);
    float angle = acos( dot(-s, spotDir) );
    float cutoff = radians( clamp( Spot.cutoff, 0.0, 90.0 ) );

    if( angle < cutoff ) {

	  vec3 v = normalize(-position);
	  vec3 r = reflect(-s, n);
	  float s_dot_n = max(dot(s, n), 0.0);

	  return color * s_dot_n + (s_dot_n > 0 ? color * pow(max(dot(r, v), 0.0), Shininess) : vec3(0.0));
	}
	return vec3(0.0);
}

vec3 ads( )
{
  vec3 n = normal;

  if (!gl_FrontFacing)
	n = -n;

  vec3 s = normalize(light_position - position);

  vec3 v = normalize(-position);
  vec3 r = reflect(-s, n);
  float s_dot_n = max(dot(s, n), 0.0);

  return color * s_dot_n + (s_dot_n > 0 ? color * pow(max(dot(r, v), 0.0), Shininess) : vec3(0.0));
}

void main() {
    vec4 t_color = texture2D(s_texture, texture_coord);

	FragColor = vec4(adss() + ads(), 1.0) * t_color; 
}
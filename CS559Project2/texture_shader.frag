#version 400

layout (location = 0) out vec4 FragColor;

uniform vec3 light_position;
uniform sampler2D s_texture;

flat in vec3 color;
in vec3 position;
in vec3 normal;
in vec2 texture_coord;

const float shininess = 10.0f;

vec3 ads( )
{
  vec3 n = normal;

  if (!gl_FrontFacing)
	n = -n;

  vec3 s = normalize(light_position - position);

  vec3 v = normalize(-position);
  vec3 r = reflect(-s, n);
  float s_dot_n = max(dot(s, n), 0.0);

  return color * s_dot_n + (s_dot_n > 0 ? color * pow(max(dot(r, v), 0.0), shininess) : vec3(0.0));
}

void main()
{
    vec4 t_color = texture2D(s_texture, texture_coord);
	FragColor = vec4(ads(), 1.0) * t_color;
}

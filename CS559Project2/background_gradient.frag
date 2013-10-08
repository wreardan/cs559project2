#version 400

/*	Perry Kivolowitz - UW - Madison - CS 559 Demo
	Background Shader
*/

layout (location = 0) out vec4 FragColor;

uniform vec4 color_array[4];
uniform ivec2 size;

/*	Given four colors in color_array, this shader draws a gradient
	across the fragments being drawn. It is intended that the entire
	window will be rasterized.

	t gets a normalized "address" of the current fragment.
	ct gets a lerp of top two colors in the horizontal direction
	cb gets a lerp of bottom two colors in the horizontal direction
	FragColor gets a lerp of ct and cb based on the vertical value of t
*/

void main()
{
	vec2 t = vec2(gl_FragCoord) / vec2(size);
	vec4 ct = mix(color_array[0], color_array[1], t.x);
	vec4 cb = mix(color_array[2], color_array[3], t.x);
	FragColor = mix(ct, cb, t.y);
}

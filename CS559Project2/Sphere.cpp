#define _USE_MATH_DEFINES
#include <math.h>
#include "Sphere.h"

using namespace std;

void Sphere::BuildPrimitive(float radius, unsigned int columns, unsigned int rows) {
	float const R = 1.0f / (float)(rows - 1);
	float const S = 1.0f / (float)(columns - 1);
	int index;
	vec3 circlePos = vec3(0, 0, 0);

	index = 0;
	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < columns; ++c) {
			float const x = cos(2 * M_PI * c * S) * sin( M_PI * r * R );
			float const y = sin( -M_PI_2 + M_PI * r * R );
			float const z = sin(2*M_PI * c * S) * sin( M_PI * r * R );
			float col_g;
			float col_r;
			if (r != 0) {
				col_g =  0;
				col_r = 1;
			} else {
				col_g = 1;
				col_r = 0;
			}
			circlePos = vec3(x * radius, y * radius, z * radius);
			this->vertices[index].position = circlePos;
			this->vertices[index].color = vec3(col_r, col_g, 0.0f);
			index++;
		}
	}
}

Sphere::Sphere()
{
}

Sphere::~Sphere()
{
}

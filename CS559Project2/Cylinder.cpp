#define _USE_MATH_DEFINES
#include <math.h>
#include "Cylinder.h"

using namespace std;

void Cylinder::BuildPrimitive(float radius, int columns, int rows, float height) {
	float const R = 1.0f / (float)(rows - 1);
	float const S = 1.0f / (float)(columns - 1);
	int index;
	vec3 circlePos = vec3(0, 0, 0);

	index = 0;
	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < columns; ++c) {
			float const x = (float)cos(2 * M_PI * c * S);
			float const y = r * R * height;
			float const z = (float)sin(2*M_PI * c * S);
			circlePos = vec3(x * radius, y * radius, z * radius);
			this->vertices[index].position = circlePos;
			this->vertices[index].color = vec3(1.0f, 0.0f, 0.0f);
			index++;
		}
	}
}

Cylinder::Cylinder()
{
}

Cylinder::~Cylinder()
{
}

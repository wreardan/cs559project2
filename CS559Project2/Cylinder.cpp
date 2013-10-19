#define _USE_MATH_DEFINES
#include <math.h>
#include "Cylinder.h"

using namespace std;

void Cylinder::BuildPrimitive(float radius, unsigned int columns, unsigned int rows) {
	float const S = 1.0f/(float)(columns-1);
	unsigned int s;
	int index = 0;
	for(s = 0; s < columns; s++) { 
		for(int h = -1; h < 2; h+=2) {
			float const y = h * 30;
			float const x = cos(2*M_PI * s * S);
			float const z = sin(2*M_PI_2 * s * S);

			vec3 vertex = vec3(x * radius, y * radius, z * radius);
			this->vertices[index].position = vertex;
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

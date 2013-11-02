#define _USE_MATH_DEFINES
#include <math.h>
#include "Sphere.h"

using namespace std;

void Sphere::BuildPrimitive(float radius, int columns, int rows) {
	float const R = 1.0f / (float)(rows - 1);
	float const S = 1.0f / (float)(columns - 1);
	int index;
	vec3 circlePos = vec3(0, 0, 0);

	index = 0;
	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < columns; ++c) {
			float const x = (float)(cos(2 * M_PI * c * S) * sin( M_PI * r * R ));
			float const y = (float)(sin( -M_PI_2 + M_PI * r * R ));
			float const z = (float)(sin(2*M_PI * c * S) * sin( M_PI * r * R ));
			/*float col_g;	GAHHHHHHHHHHHHHHHHHHHHHHHH
			float col_r;
			if (r != 0) {
				col_g =  0;
				col_r = 1;
			} else {
				col_g = 1; <=== background gradient color when we first started...
				col_r = 0;
			}*/
			circlePos = vec3(x * radius, y * radius, z * radius);
			this->vertices[index].position = circlePos;
			this->vertices[index].color = vec3(1.0f, 0.0f, 0.0f);
			index++;
		}
	}
}

void Sphere::CalculateNormals(int columns, int rows)
{
	super::CalculateNormals(columns, rows);

	int s, index;
	//top pointing up/down
	for(s = 0; s < columns; s++) {
		index = s;
		vertices[index].normal = vec3(0.0f, -1.0f, 0.0f);
		BuildNormalVisualizationGeometry(index);
		index = (rows-1)*columns + s;
		vertices[index].normal = vec3(0.0f, 1.0f, 0.0f);
		BuildNormalVisualizationGeometry(index);
	}
}

Sphere::Sphere()
{
}

Sphere::~Sphere()
{
}

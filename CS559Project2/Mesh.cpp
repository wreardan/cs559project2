#include "Mesh.h"

#include <math.h>

using namespace std;
using namespace glm;

Mesh::Mesh(void)
{
}


Mesh::~Mesh(void)
{
}

//Add a sphere to the mesh, based on this code:
//http://stackoverflow.com/questions/7946770/calculating-a-sphere-in-opengl
void Mesh::AddSphere(float radius, unsigned int rings, unsigned int sectors)
{
	const float PI = atan(1.0f)*4;
	const float PI_2 = atan(1.0f)*4/2;

	float const R = 1.0f/(float)(rings-1);
	float const S = 1.0f/(float)(sectors-1);
	unsigned int r, s;

	vertices.resize(rings * sectors);
	normals.resize(rings * sectors);
	texcoords.resize(rings * sectors);
	vector<vec3>::iterator v = vertices.begin();
	vector<vec3>::iterator n = normals.begin();
	vector<vec2>::iterator t = texcoords.begin();
	for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
		float const y = sin( -PI_2 + PI * r * R );
		float const x = cos(2*PI * s * S) * sin( PI * r * R );
		float const z = sin(2*PI * s * S) * sin( PI * r * R );

		*t = vec2(s*S, r*R);
		t++;
		//*t++ = s*S;
		//*t++ = r*R;

		*v = vec3(x * radius, y * radius, radius);
		v++;
		/**v++ = x * radius;
		*v++ = y * radius;
		*v++ = z * radius;*/

		*n = vec3(x, y, z);
		n++;
		/**n++ = x;
		*n++ = y;
		*n++ = z;*/
	}

	indices.resize(rings * sectors * 4);
	vector<unsigned short>::iterator i = indices.begin();
	for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
		*i++ = r * sectors + s;
		*i++ = r * sectors + (s+1);
		*i++ = (r+1) * sectors + (s+1);
		*i++ = (r+1) * sectors + s;
	}
}
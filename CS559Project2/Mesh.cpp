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

	//Create Vertices
	float const R = 1.0f/(float)(rings-1);
	float const S = 1.0f/(float)(sectors-1);
	unsigned int r, s;
	for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
		float const y = sin( -PI_2 + PI * r * R );
		float const x = cos(2*PI * s * S) * sin( PI * r * R );
		float const z = sin(2*PI * s * S) * sin( PI * r * R );

		vec3 vertex = vec3(x * radius, y * radius, z * radius);

		vec3 normal = vec3(x, -y, z);

		vec3 color = vec3(1.0f, 0.6f, 0.1f);
		
		vec2 tex = vec2(s*S, r*R);

		this->vertices.push_back(VertexAttributesPCNT(vertex, color, normal, tex));
	}

	//Create Indices
	vertex_indices.resize(rings * sectors * 6);
	vector<unsigned int>::iterator i = vertex_indices.begin();
	for(r = 0; r < rings-1; r++) for(s = 0; s < sectors-1; s++) {
		*i++ = r * sectors + s;
		*i++ = r * sectors + (s+1);
		*i++ = (r+1) * sectors + (s+1);
		
		*i++ = (r+1) * sectors + s;
		*i++ = r * sectors + s;
		*i++ = (r+1) * sectors + (s+1);
		
	}
}
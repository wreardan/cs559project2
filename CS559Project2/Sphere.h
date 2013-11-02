#pragma once
#include "mesh.h"
class Sphere :
	public Mesh
{
public:
	Sphere();
	~Sphere(void);
	void BuildPrimitive(float radius, int columns, int rows);
	void CalculateNormals(int columns, int rows);

protected:
	typedef Mesh super;
	void load_file(char * filename);
	GLuint texture_id;
};
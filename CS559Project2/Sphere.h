#pragma once
#include "mesh.h"
class Sphere :
	public Mesh
{
public:
	Sphere();
	~Sphere(void);
	void BuildPrimitive(float radius, unsigned int columns, unsigned int rows);

protected:
	typedef Mesh super;
	void load_file(char * filename);
	GLuint texture_id;
};
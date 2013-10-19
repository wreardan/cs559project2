#pragma once
#include "mesh.h"
class Cylinder :
	public Mesh
{
public:
	Cylinder();
	~Cylinder(void);
	void BuildPrimitive(float radius, unsigned int columns, unsigned int rows);

protected:
	typedef Mesh super;
	void load_file(char * filename);
	GLuint texture_id;
};
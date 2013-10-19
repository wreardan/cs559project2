#pragma once
#include "mesh.h"
#include "Sphere.h"
#include "Cylinder.h"

class Mars :
	public Sphere
{
public:
	Mars(char * filename);
	Mars();
	~Mars(void);

protected:
	typedef Mesh super;
	void load_file(char * filename);
	GLuint texture_id;
};


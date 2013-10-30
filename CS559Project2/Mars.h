#pragma once
#include "mesh.h"
#include "Sphere.h"
#include "Cylinder.h"

class Mars :
	public Sphere
{
public:
	Mars();
	~Mars(void);
	bool Initialize(float size, char * mars_filename, char * mars_texture_filename);

protected:
	typedef Sphere super;
	void load_file(char * filename);
	
};


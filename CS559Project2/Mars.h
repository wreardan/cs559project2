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
	bool Initialize(float size);

protected:
	typedef Sphere super;
	void load_file(char * filename);
	
};


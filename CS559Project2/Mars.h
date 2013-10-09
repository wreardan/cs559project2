#pragma once
#include "mesh.h"
class Mars :
	public Mesh
{
public:
	Mars(char * filename);
	~Mars(void);

protected:
	typedef Mesh super;
};


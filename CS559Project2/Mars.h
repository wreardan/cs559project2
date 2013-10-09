#pragma once
#include "mesh.h"
class Mars :
	public Mesh
{
public:
	Mars(char * filename);
	Mars();
	~Mars(void);

protected:
	typedef Mesh super;
	void load_file(char * filename);
};


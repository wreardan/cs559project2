#include "Error.h"
#include <GL/freeglut.h>
#include <iostream>

bool CheckGLErrors(char * s)
{
	bool return_error = false;
	GLenum glerror;

	while ((glerror = glGetError()) != GL_NO_ERROR)
	{
		return_error = true;
		std::cout << s << ": " << gluErrorString(glerror) << std::endl;
	}

	return return_error;
}
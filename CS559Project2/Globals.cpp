#include "Globals.h"
#include <gl/freeglut.h>
#include <iostream> //cin and cout

using namespace glm;
using namespace std;

Globals::Globals(void)
{
	//Initialize global variables
	window_size = ivec2(1024, 768);
	draw_planes = false;
	paused = false;
	wireframe = false;
	window_handle = -1;
}


Globals::~Globals(void)
{
}
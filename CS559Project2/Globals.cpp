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

//Draw X, Y, Z Plane Lines
//jacked parts from http://cs.lmu.edu/~ray/notes/openglexamples/
//modified to show both positive and negative axis in slightly different colors
void Globals::drawPlanes()
{
	glBegin(GL_LINES);
		//Positive Axis
		glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(100, 0, 0);
		glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 100, 0);
		glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 100);
		//Negative Axis
		glColor3f(0.2f, 0, 0); glVertex3f(0, 0, 0); glVertex3f(-100, 0, 0);
		glColor3f(0, 0.2f, 0); glVertex3f(0, 0, 0); glVertex3f(0, -100, 0);
		glColor3f(0, 0, 0.2f); glVertex3f(0, 0, 0); glVertex3f(0, 0, -100);
		glEnd();

	glBegin(GL_LINES);
	glEnd();
}
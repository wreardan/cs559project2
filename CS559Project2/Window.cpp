#include "Window.h"

Window::Window()
{
	this->time_last_pause_began = this->total_time_paused = 0;
	this->normals = this->wireframe = this->paused = false;
	this->wireframe = false;
	this->slices = 20;
	this->interval = 1000 / 120;
	this->window_handle = -1;
	mode = 3;
}
Window::~Window()
{
}

//Draw X, Y, Z Plane Lines
//jacked parts from http://cs.lmu.edu/~ray/notes/openglexamples/
//modified to show both positive and negative axis in slightly different colors
void Window::drawPlanes()
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
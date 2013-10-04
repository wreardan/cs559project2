/*
CS 559 - Computer Graphics - Project 2
(c) Fall 2013 - Wesley Reardan and Emanuel Rosu

Primary objectives
1) Draw a triangle with modern GL (main.cpp)
2) Convert P1 ship to modern GL (ship.cpp)
3) Create the Mars Sphere (mars.cpp)
	a) Basic spherical model
	b) Add altitude using mars.txt
	c) 
4) 

*/

#include <gl/glew.h>	//GL extension wrangler (shaders)
#include <gl/freeglut.h> //FreeGlut library
#include <glm/glm.hpp> //GLM Data structure
#include <glm/gtc/matrix_transform.hpp> //GLM Matrix Transforms
#include <glm/gtc/type_ptr.hpp> //GLM class to float pointer conversion
#include <iostream> //cin and cout
#include <vector> //managed array list
#include <assert.h>

#include "Globals.h"
#include "Shader.h"
#include "Error.h"

using namespace std;

Globals globals;
//Draw a square for testing shaders
void draw_square()
{
	if(globals.shader.program_id == -1)
		return;

    GLfloat vertices[] = { 0.0f,  0.0f,  // Position 0
        0.0f, 0.0f,				// TexCoord 0
        0.0f, 1.0f,			// Position 1
        0.0f, 1.0f,				// TexCoord 1
        1.0f, 1.0f,			// Position 2
        1.0f, 1.0f,				// TexCoord 2
        1.0f, 0.0f,			// Position 3
        1.0f, 0.0f,				// TexCoord 3
    };
    GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

    globals.shader.Activate();

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), vertices);
    //glVertexAttribPointer ( 1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), &vertices[2] );
    glEnableVertexAttribArray ( 0 );
    //glActiveTexture ( GL_TEXTURE0 );
    //glBindTexture ( GL_TEXTURE_2D, this->texture );
    //glUniform1i ( 2, 0 );
    glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );

	globals.shader.Deactivate();

}
void DisplayFunc()
{
	//Clear screen to black
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Load projection matrix
	//Load modelview matrix
		//Look at model
	//Draw Ship
	draw_square();

	if(globals.draw_planes)
		globals.drawPlanes();

	CheckGLErrors("DisplayFunc()");

	//Post to screen, refresh
	glutSwapBuffers();
	glutPostRedisplay();
}
void ReshapeFunc(int w, int h)
{
}
void KeyboardSpecialFunc(int c, int x, int y)
{
	switch(c)
	{
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
	case GLUT_KEY_PAGE_UP:
		break;
	case GLUT_KEY_PAGE_DOWN:
		break;
	default:
		cout << "unhandled special keyboard code: " << c << endl;
		break;
	}
}
void KeyboardFunc(unsigned char c, int x, int y)
{
	switch(c)
	{
	case 'o':
		globals.draw_planes = !globals.draw_planes;
		break;
	case 'x':
	case 27:
		glutLeaveMainLoop();
		return;
	default:
		cout << "unhandled keyboard code: " << c << endl;
		break;
	}
}
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1024, 768);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	globals.window_handle = glutCreateWindow("CS 559 Project 2");
	glutDisplayFunc(DisplayFunc);
	glutReshapeFunc(ReshapeFunc);
	glutKeyboardFunc(KeyboardFunc);

	assert(glewInit() == GLEW_OK);
	assert(globals.shader.Initialize("basic.vert", "basic.frag"));

	glutMainLoop();

	return 0;
}
/*	A more sophisticated example of modern OpenGL
	Perry Kivolowitz - UW - Madison - CS 559 demo

	In this example program, I will build a solid
	object comprised of two triangle fans. Note 
	these are topologically triangle fans but the
	OpenGL triangle fan functionality is not being
	used. 

	Created:	02/28/13
	Updates:	03/05/13 - continued improvements
				Added solid color shader for drawing normals
				Switched to timer based redisplay.
*/

#include <iostream>
#include <assert.h>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "background.h"
#include "top.h"
#include "Mesh.h"
#include "Mars.h"
#include "Ship.h"
#include "Camera.h"

using namespace std;
using namespace glm;

class Window
{
public:
	Window()
	{
		this->time_last_pause_began = this->total_time_paused = 0;
		this->normals = this->wireframe = this->paused = false;
		this->slices = 20;
		this->interval = 1000 / 120;
		this->window_handle = -1;
		mode = 2;
	}
	~Window()
	{
	}

	float time_last_pause_began;
	float total_time_paused;
	bool paused , wireframe, normals;
	int window_handle;
	int interval;
	int slices;
	ivec2 size;
	float window_aspect;
	vector<string> instructions;
	int mode;

	Background background;
	Top top;
	Mesh mesh;
	Ship ship;
	Mars mars;
} window;

void DisplayInstructions()
{
	if (window.window_handle == -1)
		return;

	vector<string> * s = &window.instructions;
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glColor3f(1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, window.size.x, 0, window.size.y, 1, 10);
	glViewport(0, 0, window.size.x, window.size.y);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(10, 15 * s->size(), -5.5);
	glScaled(0.1, 0.1, 1.0);
	for (auto i = s->begin(); i < s->end(); ++i)
	{
		glPushMatrix();
		glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char *) (*i).c_str());
		glPopMatrix();
		glTranslated(0, -150, 0);
	}
}

void CloseFunc()
{
	window.window_handle = -1;
	window.background.TakeDown();
	window.top.TakeDown();
	window.ship.TakeDown();
	window.mars.TakeDown();
	Camera::TakeDownCamera();
}

void ReshapeFunc(int w, int h)
{
	// Question for reader: Why is this 'if' statement here?
	if (h > 0)
	{
		window.size = ivec2(w, h);
		window.window_aspect = float(w) / float(h);
	}
}

void KeyboardFunc(unsigned char c, int x, int y)
{
	float current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	switch (c)
	{
	case 'S':
	case 's':
		window.top.StepShader();
		window.ship.StepShader();
		window.mars.StepShader();
		break;
		
	case 'N':
	case 'n':
		window.top.EnableNormals(window.normals = !window.normals);
		window.ship.EnableNormals(window.normals = !window.normals);
		window.mars.EnableNormals(window.normals = !window.normals);
		break;
		
	case 'W':
	case 'w':
		window.wireframe = !window.wireframe;
		break;
		
	case 'P':
	case 'p':
		if (window.paused == true)
		{
			// Will be leaving paused state
			window.total_time_paused += (current_time - window.time_last_pause_began);
		}
		else
		{
			// Will be entering paused state
			window.time_last_pause_began = current_time;
		}
		window.paused = !window.paused;
		break;
		
	case 'X':
	case 'x':
	case 27:
		glutLeaveMainLoop();
		return;
	}
}

void SpecialFunc(int c, int x, int y)
{
	switch (c)
	{
	case GLUT_KEY_UP:
		++window.slices;
		window.top.TakeDown();
		window.top.Initialize(window.slices);
		break;

	case GLUT_KEY_DOWN:
		if (window.slices > 1)
		{
			--window.slices;
			window.top.TakeDown();
			window.top.Initialize(window.slices);
		}
		break;

	case GLUT_KEY_F1:
		window.mode++;
		if(window.mode > 2)	//change to add more modes
			window.mode = 0;
		break;

	default:
		cout << "unhandled special key: " << c << endl;
	}
}

void DisplayFunc()
{
	float current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	glEnable(GL_CULL_FACE);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window.size.x, window.size.y);
	window.background.Draw(window.size);
	float time = (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused;
	Camera::Update(time);
	mat4 projection = perspective(25.0f, window.window_aspect, 1.0f, 10.0f);
	mat4 modelview = Camera::GetView();
	
	// glPolygonMode is NOT modern OpenGL but will be allowed in Projects 2 and 3
	glPolygonMode(GL_FRONT_AND_BACK, window.wireframe ? GL_LINE : GL_FILL);
	
	switch(window.mode)
	{
	case 0:
		window.top.Draw(projection, modelview, window.size, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		break;
	case 1:
		window.ship.Draw(projection, modelview, window.size, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		break;
	case 2:
		window.mars.Draw(projection, modelview, window.size, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		break;
	case 3:
		window.mesh.Draw(projection, modelview, window.size, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		break;
	default:
		cout << "DisplayFunc() unsupported display mode: " << window.mode << endl;
		window.mode = 0;
		break;
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	DisplayInstructions();
	glFlush();
}

void TimerFunc(int value)
{
	// Question for reader: Why is this 'if' statement here?
	if (window.window_handle != -1)
	{
		glutTimerFunc(window.interval, TimerFunc, value);
		glutPostRedisplay();
	}
}

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);

	window.window_handle = glutCreateWindow("CS 559 Project 2 Fall 2013");
	glutReshapeFunc(ReshapeFunc);
	glutCloseFunc(CloseFunc);
	glutDisplayFunc(DisplayFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);
	glutTimerFunc(window.interval, TimerFunc, 0);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	window.instructions.push_back("CS 559 Project 2");
	window.instructions.push_back("Wesley Reardan, Emanuel Rosu");
	window.instructions.push_back("F1 - Change Display Mode");
	window.instructions.push_back("W - Wireframe Mode");
	window.instructions.push_back("S - Cycle Shaders");
	window.instructions.push_back("P - Pause");

	if (glewInit() != GLEW_OK)
	{
		cerr << "GLEW failed to initialize." << endl;
		return 0;
	}

	if (!window.background.Initialize())
		return 0;
	if (!window.top.Initialize(window.slices))
		return 0;
	if(!window.ship.Initialize((float)window.slices))
		return 0;
	if(!window.mars.Initialize((float)window.slices))
		return 0;
	if(!Camera::Initialize()) {
		return 0;
	}

	glutMainLoop();
}

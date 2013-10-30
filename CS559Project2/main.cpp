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

#include "Window.h"

using namespace std;
using namespace glm;

Window window;

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
	case 'V':
	case 'v':
		window.camera.SetCameraType((window.camera.GetCameraType() == Camera::Type::normal) ? Camera::Type::chase : Camera::Type::normal);
		break;
	case 'S':
		window.top.StepShader();
		window.mars.StepBackShader();
		window.ship.StepShader();
		break;
	case 's':
		window.top.StepShader();
		window.mars.StepShader();
		window.ship.StepShader();
		break;
		
	case 'N':
	case 'n':
		window.top.EnableNormals(window.normals = !window.normals);
		window.mars.EnableNormals(window.normals);
		window.ship.EnableNormals(window.normals);
		break;
		
	case 'W':
	case 'w':
		window.wireframe = !window.wireframe;
		break;

	case 'o':
	case 'O':
		window.draw_planes = !window.draw_planes;
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
		if(window.camera.up_down < 2.5f)
			window.camera.up_down += 0.01f;
		break;

	case GLUT_KEY_DOWN:
		if(window.camera.up_down > 0.0f)
			window.camera.up_down -= 0.01f;
		break;

	case GLUT_KEY_F1:
		window.mode++;
		if(window.mode > 3)	//change to add more modes
			window.mode = 0;
		break;

	case GLUT_KEY_LEFT:
		window.camera.x_offset -= 1.0f;
		break;

	case GLUT_KEY_RIGHT:
		window.camera.x_offset += 1.0f;
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
	window.camera.Update(time);
	mat4 projection = perspective(25.0f, window.window_aspect, 1.0f, 3000.0f);
	mat4 view = window.camera.GetView();
	window.lights.cameraMatrix = view;
	mat4 temp;
	
	// glPolygonMode is NOT modern OpenGL but will be allowed in Projects 2 and 3
	glPolygonMode(GL_FRONT_AND_BACK, window.wireframe ? GL_LINE : GL_FILL);
	
	switch(window.mode)
	{
	case 0:
		/*Just your newly improved spaceship slowly turning so we can admire your mesh
		construction and lighting correctness. With or without a starfield.*/
		window.camera.type = Camera::normal;
		window.camera.scalar = 2.0f;
		window.camera.rotation_speed = 20.0f;
		window.ship.Draw(projection, view, window.size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		break;
	case 1:
		/*Just Mars, slowly spinning as per the sample prototype I provide. With or without a starfield.*/
		if(window.camera.scalar < 12.0f) {
			window.camera.type = Camera::normal;
			window.camera.scalar = 12.0f;
			window.camera.rotation_speed = 10.0f;
			window.camera.Initialize();
		}
		window.mars.Draw(projection, view, window.size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		break;
	case 2:
		/*First person view flying over the surface of Mars. It is NOT necessary but would be bonus
		to be able to steer (bonus hint). With or without steering, the starfield must “correctly”
		turn with your motion.*/
		window.camera.type = Camera::chase;
		window.camera.rotation_speed = 10.0f;
		window.mars.Draw(projection, view, window.size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		break;
	case 3:
		/*Third person view flying over the surface of Mars from a “chase camera.” The camera
		should follow along with your spaceship. It is NOT necessary to be able to alter the
		position of the chase camera relative to the spaceship (bonus hint). Again, a “correctly”
		turning starfield is required.*/
		window.camera.type = Camera::chase;
		window.mars.Draw(projection, view, window.size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		temp = translate(mat4(1.0f), vec3(0.0f, 0.65f, -6.0f));
		temp = rotate(temp, -90.0f, vec3(1.0f, 0.0f, 0.0f));
		temp = rotate(temp, 30.0f, vec3(0.0f, 1.0f, 0.0f));
		window.ship.Draw(projection, temp, window.size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		break;
	default:
		cerr << "DisplayFunc() unsupported display mode: " << window.mode << endl;
		window.mode = 0;
		break;
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	DisplayInstructions();
	if(window.draw_planes)
		window.drawPlanes();
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

	ilInit();

	window.instructions.push_back("CS 559 Project 2");
	window.instructions.push_back("Wesley Reardan, Emanuel Rosu");
	window.instructions.push_back("F1 - Change Display Mode");
	window.instructions.push_back("W - Wireframe Mode");
	window.instructions.push_back("S - Cycle Shaders");
	window.instructions.push_back("P - Pause");
	window.instructions.push_back("V - Change Camera Mode");

	if (glewInit() != GLEW_OK)
	{
		cerr << "GLEW failed to initialize." << endl;
		return 0;
	}

	if (!window.background.Initialize())
		return 0;
	if (!window.top.Initialize(window.slices))
		return 0;
	if(!window.ship.Initialize())
		return 0;
	if(!window.mars.Initialize((float)window.slices, argv[1], "mars.jpg"))
		return 0;
	if(!window.camera.Initialize()) {
		return 0;
	}
	Light light;
	light.SetPosition(vec3(0.0f, 0.0f, 50.0f));
	window.lights.Add(light);

	glutMainLoop();
}

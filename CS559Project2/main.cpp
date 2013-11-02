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
GLuint fboHandle; // The handle to the FBO
GLuint renderTex;
GLuint depthBuf;
GLuint whiteTexHandle;


void InitFBO() {	
	// Generate and bind the framebuffer
    glGenFramebuffers(1, &fboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

    // Create the texture object
    GLuint renderTex;
    glGenTextures(1, &renderTex);
    glActiveTexture(GL_TEXTURE0);  // Use texture unit 0
    glBindTexture(GL_TEXTURE_2D, renderTex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, 1024, 768, 0, GL_RGBA,GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Bind the texture to the FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);

    // Create the depth buffer
    GLuint depthBuf;
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);

    // Bind the depth buffer to the FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depthBuf);

    // Set the targets for the fragment output variables
    GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);

        GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if( result == GL_FRAMEBUFFER_COMPLETE) {
                cout << "Framebuffer is complete" << endl;
        } else {
                cout << "Framebuffer error: " << result << endl;
        }

    // Unbind the framebuffer, and revert to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void InitWhiteTex() {
	// One pixel white texture
	GLubyte whiteTex[] = { 255, 255, 255, 255 };
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &whiteTexHandle);
	glBindTexture(GL_TEXTURE_2D, whiteTexHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA,
	GL_UNSIGNED_BYTE, whiteTex);
}

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
	window.starfield.TakeDown();
	window.rendertexture.TakeDown();
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
	case 's':
		window.top.StepShader();
		window.mars.StepShader();
		window.ship.StepShader();
		window.rendertexture.StepShader();
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
		if(window.mode > 4)	//change to add more modes
			window.mode = 0;
		if(window.mode == 4) {
			window.camera.scalar = 10000.0f;
			window.camera.Initialize();
		}
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

void RenderToTexture(float current_time) {
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
	
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	
	glViewport(0, 0, window.size.x, window.size.y);
	
	window.background.Draw(window.size);
	float time = (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused;
	window.camera.Update(time);
	mat4 projection = perspective(25.0f, window.window_aspect, 1.0f, 3000.0f);
	mat4 view = window.camera.GetView();
	window.lights.cameraMatrix = view;
	window.lights.normalMatrix = mat3(inverse(transpose(view)));
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
		window.starfield.Update();
		window.starfield.Draw(projection, view, window.size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
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
		window.starfield.Update();
		window.starfield.Draw(projection, view, window.size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		window.mars.Draw(projection, view, window.size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		break;
	case 2:
		/*First person view flying over the surface of Mars. It is NOT necessary but would be bonus
		to be able to steer (bonus hint). With or without steering, the starfield must “correctly”
		turn with your motion.*/
		window.camera.type = Camera::chase;
		window.camera.rotation_speed = 10.0f;
		window.starfield.Update();
		window.starfield.Draw(projection, view, window.size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
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
		window.starfield.Update();
		window.starfield.Draw(projection, view, window.size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		window.ship.Draw(projection, temp, window.size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		break;
	case 4:
		window.camera.type = Camera::normal;
		window.camera.rotation_speed = 10.0f;
		window.starfield.Update();
		window.starfield.Draw(projection, view, window.size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		break;
	default:
		cerr << "DisplayFunc() unsupported display mode: " << window.mode << endl;
		window.mode = 0;
		break;
	}
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	DisplayInstructions();
	
	if(window.draw_planes) {
		window.drawPlanes();
	}
	
	//Unbind FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderScene(float current_time) {
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, window.size.x, window.size.y);
	
	float time = (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused;
	mat4 projection = perspective(25.0f, window.window_aspect, 1.0f, 3000.0f);
	vec3 facing = vec3(0.0f, 0.0f, 0.0f);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 position = vec3(0.0f, 0.0f, -4.0f);
	mat4 view = lookAt(position, facing, up);


	//view = rotate(view, 180.0f, vec3(0, 0, 0));
	view = scale(view, vec3(0.145f, 0.11f, 0.10f));
	view = translate(view, vec3(-50.0f, -50.0f, 200.0f));
	
	window.rendertexture.Draw(projection, view, window.size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);	
	glFlush();
}

void DisplayFunc()
{
	float current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	 glBindTexture(GL_TEXTURE_2D, 0);
     glEnable(GL_TEXTURE_2D);
	//bind FBO
	
	RenderToTexture(current_time);
	RenderScene(current_time);
	
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
	if(!window.starfield.Initialize()) {
		return 0;
	}
	if(!window.rendertexture.Initialize()) {
		return 0;
	}
	Light light, spotlight;
	light.SetPosition(vec4(0.0f, 0.0f, 50.0f, 1.0f));
	window.lights.Add(light);
	spotlight.SetPosition(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	spotlight.direction = vec3(0.0f, -0.1f, -1.0f);
	window.lights.Add(spotlight);
	
	InitFBO();
	InitWhiteTex();

	glutMainLoop();
}

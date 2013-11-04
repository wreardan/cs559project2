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
GLuint whiteTexHandle;

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
	case 'Z':
	case 'z':
		window.ship.StepShader();
		break;
	case 'S':
	case 's':
		window.top.StepShader();
		window.mars.StepObjectShader();
		break;
	case 'Q':
	case 'q':
		window.rendertexture.StepPostEffect();
		break;
	case 'N':
	case 'n':
		window.top.EnableNormals(window.normals = !window.normals);
		window.mars.EnableNormals(window.normals);
		window.ship.EnableNormals(window.normals);
		break;	
	case 'W':
		window.mars.wireframe_mode++;
		if(window.mars.wireframe_mode > 3)
			window.mars.wireframe_mode = 0;
		window.ship.sphere.wireframe_mode = window.mars.wireframe_mode;
		break;
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
	case 'U':
	case 'u':
		window.lights.lights[0].SetPosition(vec4(window.lights.lights[0].GetPosition().x, window.lights.lights[0].GetPosition().y + 1.0f, window.lights.lights[0].GetPosition().z, 1.0f));
		break;
	case 'J':
	case 'j':
		window.lights.lights[0].SetPosition(vec4(window.lights.lights[0].GetPosition().x, window.lights.lights[0].GetPosition().y - 1.0f, window.lights.lights[0].GetPosition().z, 1.0f));
		break;
	case 'H':
	case 'h':
		window.lights.lights[0].SetPosition(vec4(window.lights.lights[0].GetPosition().x - 1.0f, window.lights.lights[0].GetPosition().y, window.lights.lights[0].GetPosition().z, 1.0f));
		break;
	case 'K':
	case 'k':
		window.lights.lights[0].SetPosition(vec4(window.lights.lights[0].GetPosition().x + 1.0f, window.lights.lights[0].GetPosition().y, window.lights.lights[0].GetPosition().z, 1.0f));
		break;
	case 'T':
	case 't':
		window.lights.lights[0].SetPosition(vec4(window.lights.lights[0].GetPosition().x, window.lights.lights[0].GetPosition().y, window.lights.lights[0].GetPosition().z + 1.0f, 1.0f));
		break;
	case 'G':
	case 'g':
		window.lights.lights[0].SetPosition(vec4(window.lights.lights[0].GetPosition().x, window.lights.lights[0].GetPosition().y, window.lights.lights[0].GetPosition().z + 1.0f, 1.0f));
		break;
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

void UpdateScene(float current_time) {
	float time = (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused;
	window.camera.Update(time);
}

//https://github.com/daw42/glslcookbook/blob/master/chapter07/sceneshadowmap.cpp
void spitOutDepthBuffer(int width, int height) {
    int size = width * height;
    float * buffer = new float[size];
    unsigned char * imgBuffer = new unsigned char[size * 4];

    glGetTexImage(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,GL_FLOAT,buffer);

    for( int i = 0; i < height; i++ )
        for( int j = 0; j < width; j++ )
        {
            int imgIdx = 4 * ((i*width) + j);
            int bufIdx = ((height - i - 1) * width) + j;

            // This is just to make a more visible image.  Scale so that
            // the range (minVal, 1.0) maps to (0.0, 1.0).  This probably should
            // be tweaked for different light configurations.
            float minVal = 0.88f;
            float scale = (buffer[bufIdx] - minVal) / (1.0f - minVal);
            unsigned char val = (unsigned char)(scale * 255);
            imgBuffer[imgIdx] = val;
            imgBuffer[imgIdx+1] = val;
            imgBuffer[imgIdx+2] = val;
            imgBuffer[imgIdx+3] = 0xff;
        }

//    QImage img(imgBuffer, shadowMapWidth, shadowMapHeight, QImage::Format_RGB32);
//    img.save("depth.png", "PNG");
//http://bobobobo.wordpress.com/2009/03/02/how-to-use-openil-to-generate-and-save-an-image/
	ILuint imageID = ilGenImage();
	ilBindImage(imageID);
	ilTexImage(width, height, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, imgBuffer);
	ilEnable(IL_FILE_OVERWRITE);
	ilSave( IL_PNG, "depth_buffer.png" ) ;

    delete [] buffer;
    delete [] imgBuffer;
    exit(1);
}


void RenderToTexture(float current_time, mat4 projection, mat4 view) {
//	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	ivec2 win_size = ivec2(1024, 768);

	glViewport(0, 0, win_size.x, win_size.y);
	
	window.background.Draw(win_size);
	float time = (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused;
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
		if(window.camera.scalar > 1000.0f) {
			window.camera.type = Camera::normal;
			window.camera.scalar = 2.0f;
			window.camera.Initialize();
		}
		window.starfield.Update(false);
		window.starfield.Draw(projection, view, win_size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		window.camera.rotation_speed = 20.0f;
		window.ship.Draw(projection, view, win_size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		break;
	case 1:
		/*Just Mars, slowly spinning as per the sample prototype I provide. With or without a starfield.*/
		if(window.camera.scalar < 12.0f) {
			window.camera.type = Camera::normal;
			window.camera.scalar = 12.0f;
			window.camera.rotation_speed = 10.0f;
			window.camera.Initialize();
		}
		window.starfield.Update(false);
		window.starfield.Draw(projection, view, win_size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		window.mars.Draw(projection, view, win_size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		break;
	case 2:
		/*First person view flying over the surface of Mars. It is NOT necessary but would be bonus
		to be able to steer (bonus hint). With or without steering, the starfield must “correctly”
		turn with your motion.*/
		if (window.camera.type != Camera::chase) {
			window.camera.x_offset = 0.0f;
			window.camera.up_down = 1.5f;
		}
		window.camera.type = Camera::chase;
		window.camera.rotation_speed = 10.0f;
		window.starfield.Update(false);
		window.starfield.Draw(projection, view, win_size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		window.mars.Draw(projection, view, win_size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		break;
	case 3:
		/*Third person view flying over the surface of Mars from a “chase camera.” The camera
		should follow along with your spaceship. It is NOT necessary to be able to alter the
		position of the chase camera relative to the spaceship (bonus hint). Again, a “correctly”
		turning starfield is required.*/
		if (window.camera.type != Camera::ship) {
			window.camera.x_offset = 0.0f;
		}
		window.camera.type = Camera::ship;
		window.mars.Draw(projection, view, win_size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		temp = translate(mat4(1.0f), vec3(0.0f, 0.30f, -3.0f));
		temp = rotate(temp, -70.0f, vec3(1.0f, 0.0f, 0.0f));
		temp = rotate(temp, 30.0f, vec3(0.0f, 1.0f, 0.0f));
		temp = scale(temp, vec3(0.25f, 0.25f, 0.25f));
		//temp = rotate(temp, 67.0f, vec3(1.0f, 0.0f, 0.0f));
		window.starfield.Update(false);
		window.starfield.Draw(projection, view, win_size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		window.ship.Draw(projection, temp, win_size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		break;
	case 4:
		/*Just the starfield slowly turning when viewed from outside the “universe”. That is, the
		virtual camera is placed outside a ball of turning stars.*/
		window.camera.type = Camera::normal;
		window.camera.scalar = 1200.0f;
		window.starfield.Update(true);
		window.starfield.Draw(projection, view, win_size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
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
}

void RenderScene(float current_time) {
	UpdateScene(current_time);

	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, window.size.x, window.size.y);
	
	float time = (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused;
	mat4 projection = perspective(25.0f, (float)(1024/768), 1.0f, 3000.0f); //fixed value because of our texture viewport
	vec3 facing = vec3(0.0f, 0.0f, 0.0f);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 position = vec3(0.0f, 0.0f, -4.0f);
	mat4 view = lookAt(position, facing, up);


	//view = rotate(view, 180.0f, vec3(0, 0, 0));
	view = scale(view, vec3(0.11f, 0.11f, 0.10f));
	view = translate(view, vec3(-50.0f, -50.0f, 200.0f));
	
	window.rendertexture.Draw(2, projection, view, window.size, window.lights, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);	
	glFlush();
}

void DisplayFunc()
{
	float current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	//Render scene normally to texture for post-processing
	
	mat4 projection = perspective(25.0f, window.window_aspect, 1.0f, 5000.0f);
	mat4 view = window.camera.GetView();

	window.mars.shadow_pass_type = 1;
	window.ship.sphere.shadow_pass_type = 1;

	glBindTexture(GL_TEXTURE_2D, 1);
    glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
    //glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);

	window.frame_buffer.Use();	//bind FBO

	RenderToTexture(current_time, projection, view);

	window.frame_buffer.Disable();	//Unbind FBO

	//Finally, render texture to screen using post-processing shaders
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
	window.size = vec2(1024, 768);
	window.window_aspect = 1024 / 768;

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
	window.instructions.push_back("w - Legacy wireframe mode, W - Advanced Wireframe mode (selected Shaders)");
	window.instructions.push_back("S - Cycle Planet Shader, Q - Cycle Post-Processing Effect, Z - Cycle Ship Shader");
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

	//https://github.com/daw42/glslcookbook/blob/master/chapter07/sceneshadowmap.cpp
	mat4 shadowBias = mat4( vec4(0.5f,0.0f,0.0f,0.0f),
                        vec4(0.0f,0.5f,0.0f,0.0f),
                        vec4(0.0f,0.0f,0.5f,0.0f),
                        vec4(0.5f,0.5f,0.5f,1.0f)
                        );

	Light light, spotlight;
	light.SetPosition(vec4(0.0f, 0.0f, 50.0f, 1.0f));

	window.lights.Add(light);


	spotlight.SetPosition(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	spotlight.direction = vec3(0.0f, -0.1f, -1.0f);
	window.lights.Add(spotlight);

	window.frame_buffer.Initialize(window.size.x, window.size.y);
	
	InitWhiteTex();

	glutMainLoop();
}

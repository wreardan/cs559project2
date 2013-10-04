#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
class Globals
{
public:
	Globals(void);
	~Globals(void);

//Global Variables
	glm::ivec2 window_size;
	bool draw_planes;		//should we draw x,y,z axis or not?
	bool paused;			//is rotation paused?
	bool wireframe;			//is wireframe mode enabled?
	int window_handle;
	Shader shader;

//Global functions
	void drawPlanes();
};


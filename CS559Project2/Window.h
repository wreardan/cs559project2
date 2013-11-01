#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "Camera.h"
#include "background.h"
#include "top.h"
#include "Ship.h"
#include "Mars.h"
#include "Light.h"
#include "Starfield.h"

class Window
{
public:
	Window();
	~Window();

	float time_last_pause_began;
	float total_time_paused;
	bool paused , wireframe, normals;
	int window_handle;
	int interval;
	int slices;
	glm::ivec2 size;
	float window_aspect;
	std::vector<std::string> instructions;
	int mode;

	Background background;
	Top top;
	Mesh mesh;
	Ship ship;
	Mars mars;
	Camera camera;
	Lights lights;
	Starfield starfield;
	bool draw_planes;		//should we draw x,y,z axis or not?
	void drawPlanes();
};
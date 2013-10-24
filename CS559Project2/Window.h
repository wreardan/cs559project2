#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "Camera.h"
#include "background.h"
#include "top.h"
#include "Ship.h"
#include "Mars.h"

class Window
{
public:
	Window()
	{
		this->time_last_pause_began = this->total_time_paused = 0;
		this->normals = this->wireframe = this->paused = false;
		this->wireframe = false;
		this->slices = 20;
		this->interval = 1000 / 120;
		this->window_handle = -1;
		mode = 3;
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
};
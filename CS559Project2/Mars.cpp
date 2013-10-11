#include "Mars.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/freeglut.h>
using namespace std;

#include <SOIL.h>

void Mars::load_file(char * filename)
{
	const float scalar = 0.05f;	//scalar for mars depth data
	//Load file, error if cannot open
	string line;
	stringstream line_stream;
	ifstream file(filename);
	float depth;

	int width; 
	int height; 
	if(!file.is_open()) {
		cerr << "could not open mars.txt file" << endl;
		width = 20;
		height = 20;
	}
	else {
		//Read first line with longitude and latitude information
		getline(file, line);
		line_stream << line;
		line_stream >> width;
		line_stream >> height;
		cout << "width: " << width << ", height: " << height << endl;
	}
	
	//Create Mesh
	this->BuildSphere(1.0f, width, height);

	//Modify Mesh Vertices
	for(int y = 0; y < height; y++) {
		getline(file, line);
		line_stream.clear();
		line_stream << line;
		for(int x = 0; x < width; x++) {
			line_stream >> depth;
			depth = 1.0f - depth * scalar;
			assert(depth >= 0.0f && depth <= 1.0f);
			vertices[y*width + x].position *= depth;
		}
	}

	//Load Texture File
	/*int tex_width, tex_height;
	unsigned char * image = SOIL_load_image("Mars_2k-050104.png", &tex_width, &tex_height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);*/
	//http://www.lonesock.net/soil.html
	unsigned char* image = SOIL_load_image("img.png", &width, &height, 0, SOIL_LOAD_RGB);
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
}

//Load Mars from Filename
Mars::Mars(char * filename)
{
	load_file(filename);
}

Mars::Mars(void)
{
#ifdef _DEBUG
	load_file("mars_low_rez.txt");
#else
	load_file("mars.txt");
#endif
}

Mars::~Mars(void)
{

}

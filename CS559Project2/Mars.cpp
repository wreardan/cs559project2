#include "Mars.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

void Mars::load_file(char * filename)
{
	const float scalar = 0.025f;	//scalar for mars depth data
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
	this->BuildMesh(width, height);
	//Wrap Into Sphere
	this->BuildPrimitive(10.0f, width, height);


	//Modify Mesh Vertices
	int index = 0;
	for(int y = 0; y < height; y++) {
		getline(file, line);
		line_stream.clear();
		line_stream << line;
		for(int x = 0; x < width; x++) {
			line_stream >> depth;
			depth = 1.0f + depth * scalar;
			assert(depth >= 0.0f && depth <= 2.0f);
			vertices[index].position *= depth;
			index++;
		}
	}
	
	this->CalculateSphereNormals(width, height);
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

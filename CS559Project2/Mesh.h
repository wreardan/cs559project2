#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "vertexattributes.h"
class Mesh
{
protected:
	std::vector<VertexAttributesPCNT> vertices;
	std::vector<unsigned int> vertex_indices;

public:
	Mesh(void);
	~Mesh(void);

	void AddSphere(float radius, unsigned int rings, unsigned int sectors);
	//TODO:
	void AddCylinder(float radius, float height);
	void AddCube(float sx, float sy, float sz);
};


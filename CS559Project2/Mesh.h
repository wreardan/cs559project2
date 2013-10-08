#pragma once
#include <vector>
#include <glm/glm.hpp>
class Mesh
{
public:
	Mesh(void);
	~Mesh(void);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<unsigned short> indices;

	void AddSphere(float radius, unsigned int rings, unsigned int sectors);
	//TODO:
	void AddCylinder(float radius, float height);
	void AddCube(float sx, float sy, float sz);
};


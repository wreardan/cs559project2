#pragma once
#include "mesh.h"
#include "Sphere.h"
#include "Cylinder.h"

class Starfield :
	public Sphere
{
public:
	Starfield();
	~Starfield(void);
	bool Initialize();
	void Update(bool strength);
	void Draw(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, Lights & lights, const float time);

protected:
	typedef Sphere super;
};


#include "Ship.h"


Ship::Ship(void)
{
	const float scalar = 0.05f;	//base size
	
	//Create Mesh
	this->BuildSphere(1.0f, 20, 20);
}


Ship::~Ship(void)
{
}

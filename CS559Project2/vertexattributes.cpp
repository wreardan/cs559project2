/*	A more sophisticated Hello World using modern OpenGL
	Perry Kivolowitz - UW - Madison - CS 559 demo

	Created:	02/28/13
	Updates:
*/

#include "vertexattributes.h"

using namespace glm;

VertexAttributesPCNT::VertexAttributesPCNT()
{
	this->position = vec3(0.0f);
	this->color = vec3(0.0f);
	this->normal = vec3(0.0f);
	this->texture_coordinate = vec2(0.0f);
}

VertexAttributesPCNT::VertexAttributesPCNT(const vec3 & p, const vec3 & c, const vec3 & n, const vec2 & t)
{
	this->position = p;
	this->color = c;
	this->normal = n;
	this->texture_coordinate = t;
}

VertexAttributesPCNT::VertexAttributesPCNT(const VertexAttributesPCNT & other)
{
	this->position = other.position;
	this->color = other.color;
	this->normal = other.normal;
	this->texture_coordinate = other.texture_coordinate;
}

VertexAttributesP::VertexAttributesP(const vec3 & p)
{
	this->position = p;
}

VertexAttributesP::VertexAttributesP(const VertexAttributesP & other)
{
	this->position = other.position;
}

VertexAttributesP::VertexAttributesP()
{
	this->position = vec3(0.0f);
}

VertexAttributesPN::VertexAttributesPN(const vec3 & p, const vec3 & n)
{
	this->position = p;
	this->normal = n;
}

VertexAttributesPN::VertexAttributesPN(const VertexAttributesPN & other)
{
	this->position = other.position;
	this->normal = other.normal;
}

VertexAttributesPN::VertexAttributesPN()
{
	this->position = vec3(0.0f);
	this->normal = vec3(0.0f, 0.0f, 1.0f);
}


VertexAttributesPCN::VertexAttributesPCN()
{
	this->position = vec3(0.0f);
	this->color = vec3(0.0f);
	this->normal = vec3(0.0f);
}

VertexAttributesPCN::VertexAttributesPCN(const vec3 & p, const vec3 & c, const vec3 & n)
{
	this->position = p;
	this->color = c;
	this->normal = n;
}

VertexAttributesPCN::VertexAttributesPCN(const VertexAttributesPCN & other)
{
	this->position = other.position;
	this->color = other.color;
	this->normal = other.normal;
}

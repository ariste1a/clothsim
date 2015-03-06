#pragma once
#include "triangle.h"
class Aerodynamics
{
public:
	Aerodynamics();
	std::vector<Triangle*> triangles;
	Vector3 windVelocity; 
	void computeForces(); 
	~Aerodynamics();
};


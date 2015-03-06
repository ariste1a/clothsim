#pragma once
#include "Particle.h"
class aeroDynamics
{
public:
	aeroDynamics();
	aeroDynamics(Particle *p1, Particle *p2, Particle *p3);
	Particle *p1;
	Particle *p2;
	Particle *p3;
	float density; 
	float cd; 
	Vector3 normal;
	Vector3 airVelocity; 
	void computeForces(); 
	~aeroDynamics();
};


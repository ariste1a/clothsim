#pragma once
#include "Particle.h"
class Triangle
{
public:
	Triangle();
	Triangle(Particle *p1, Particle *p2, Particle *p3);
	Particle *p1;
	Particle *p2;
	Particle *p3;
	float density; 
	float cd; 
	Vector3 normal;
	Vector3 airVelocity; 
	void computeForces(Vector3&); 
	void computeNormal();
	Vector3 n;
	~Triangle();
};


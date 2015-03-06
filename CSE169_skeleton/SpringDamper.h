#pragma once
#include "Particle.h""
class SpringDamper
{
public:
	SpringDamper();
	SpringDamper(Particle *p1, Particle *p2);
	float springConstant; 
	float dampingFactor; 
	Particle *p1, *p2; 	
	float restLength; 
	float restLength2; 
	void computeForce(); 
	void draw(); 
	~SpringDamper();
};


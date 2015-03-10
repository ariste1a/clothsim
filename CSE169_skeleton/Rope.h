#pragma once
#include "Particle.h"
#include "SpringDamper.h"
class Rope
{
public:
	Rope();
	Rope(Particle *p1, Particle *p2, int links);
	Particle *p1; 
	Particle *p2; 	
	std::vector<Particle*> rope; 
	std::vector<SpringDamper*> dampers; 
	float length;
	int links; 
	void draw(); 
	void reset(); 
	~Rope();
};


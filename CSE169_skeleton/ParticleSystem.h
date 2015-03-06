#pragma once
#include "Particle.h"
#include "../vector3.h"
#include <vector> 
#include "SpringDamper.h"
#include "aeroDynamics.h"
class ParticleSystem
{
public:
	ParticleSystem();
	ParticleSystem(int numparticles);
	int numParticles = 20;
	std::vector<Particle*> particles; 
	Particle* clothParticles[20][20]; 
	std::vector<SpringDamper*> springDampers; 
	std::vector<aeroDynamics*> triangles;
	void update(float deltaTime); 
	void draw(); 
	void reset(); 
	~ParticleSystem();
};


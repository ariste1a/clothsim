#pragma once
#include "Particle.h"
#include "../vector3.h"
#include <vector> 
#include "SpringDamper.h"
#include "Triangle.h"
#include "Rope.h"
class Parachute
{
public:
	Parachute();
	int numParticles = 20;
	std::vector<Particle*> particles;
	Particle* clothParticles[20][20];
	std::vector<SpringDamper*> springDampers;
	std::vector<Triangle*> triangles;
	Particle* box[2][2][2]; 
	void update(float deltaTime, Vector3& wind);		
	void draw();
	void reset();
	std::vector<Rope*> ropes; 
	~Parachute();
};


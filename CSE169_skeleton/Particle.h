#pragma once
#include "../vector3.h"
#include "../matrix34.h"

class Particle
{
public:
	Particle();
	Particle(float x, float y, float z);
	float mass; 
	Vector3 position; 
	Vector3 velocity; 
	Vector3 force; 
	void update(float deltaTime); 
	void draw(); 
	void applyForce(Vector3 &f); 
	bool pinned; 
	Vector3 normal; 
	~Particle();
};


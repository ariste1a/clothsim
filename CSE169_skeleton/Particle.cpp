#include "Particle.h"


Particle::Particle()
{
	mass = 1.0;
	position = *new Vector3();
	velocity = *new Vector3(); 
	force = *new Vector3(); 
	pinned = false; 
}

void Particle::update(float deltaTime) {
	// Compute acceleration (Newton’s second law)	
	
	/* should put this somewhere else?*/

	/*
		- For each particle: Apply gravity
		- For each spring-damper: Compute & apply forces
		- For each triangle: Compute & apply aerodynamic forces		
	*/
	
	//Vector3 fAero = 0.5*rho*velocity.Mag2()*coeffDrag*(-velocity.Normalize()); 
	
	// Compute new position & velocity
	Vector3 Accel = (1.0 / mass) * force;

	velocity += Accel*deltaTime;
	position += velocity*deltaTime;

	/*if (position.y > -2)
	{
		velocity += Accel*deltaTime;
		position += velocity*deltaTime;
	}*/
	// Zero out Force vector
	force.Zero();
}

void Particle::applyForce(Vector3 &f)
{
	this->force += f; 
}

void Particle::draw()
{
	glColor3f(255, 0, 0); 
	//std::cout << this->position.x << std::endl;
	glVertex3f(this->position.x, this->position.y, this->position.z);
}
Particle::~Particle()
{
}

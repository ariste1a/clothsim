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
	Vector3 Accel = (1.0 / mass) * force;
	

	velocity += Accel*deltaTime;
	position += velocity*deltaTime;

	if (position.y <= -10)
	{
		float elasticity = 1.0; 
		float friction = 5;
		position.y = position.y + 0.01;
		velocity.y = -elasticity * velocity.y; 
		velocity.x = (1 - friction)* velocity.x;
		velocity.z = (1 - friction)* velocity.z;

		velocity.Zero();
	}
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

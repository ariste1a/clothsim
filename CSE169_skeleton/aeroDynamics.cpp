#include "aeroDynamics.h"


aeroDynamics::aeroDynamics()
{
}

aeroDynamics::aeroDynamics(Particle *p1, Particle *p2, Particle *p3)
{
	density = 1.2;
	cd = 1.1;
	airVelocity = *new Vector3(); 
}

void aeroDynamics::computeForces()
{
	Vector3 velocity = (p1->velocity + p2->velocity + p3->velocity) / 3; 
	Vector3 realVelocity = velocity - airVelocity; 

	Vector3 *one = new Vector3();	
	Vector3 *two = new Vector3();
	one->Cross(p2->position - p1->position, p3->position - p1->position);
	two->Cross(p2->position - p1->position, p3->position - p1->position);
	one->Normalize(); 
	normal = *one; 

	float a0 = 0.5 * two->Mag(); 
	float a = a0 * (velocity.Dot(normal) / velocity.Mag());

	Vector3 fAero = -0.5 * density * velocity.Mag2() *a *normal; 

	Vector3 fAeroParticle = fAero / 3; 

	p1->applyForce(fAeroParticle);
	p2->applyForce(fAeroParticle);
	p3->applyForce(fAeroParticle);

}

aeroDynamics::~aeroDynamics()
{
}

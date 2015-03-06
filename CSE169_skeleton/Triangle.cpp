#include "Triangle.h"


Triangle::Triangle()
{
}

Triangle::Triangle(Particle *p1, Particle *p2, Particle *p3)
{
	density = 1.2;
	cd = 1.1;
	airVelocity = *new Vector3(0,0, 0); 
	this->p1 = p1; 
	this->p2 = p2; 
	this->p3 = p3; 
}

void Triangle::computeForces(Vector3 &airVelocity)
{
	this->airVelocity = airVelocity;
	Vector3 velocity = (p1->velocity + p2->velocity + p3->velocity) / 3;

	Vector3 realVelocity = velocity - airVelocity;

	if (realVelocity.Mag() > 0)
	{
		Vector3 *one = new Vector3();
		Vector3 *two = new Vector3();
		one->Cross(p2->position - p1->position, p3->position - p1->position);
		two->Cross(p2->position - p1->position, p3->position - p1->position);
		one->Normalize();
		normal = *one;

		float a0 = 0.5 * two->Mag();
		float a = a0 * (realVelocity.Dot(normal) / realVelocity.Mag());

		Vector3 fAero = -0.5 * density * realVelocity.Mag2() *a *normal;

		Vector3 fAeroParticle = fAero / 3;

		if (!p1->pinned)
			p1->applyForce(fAeroParticle);
		if (!p2->pinned)
			p2->applyForce(fAeroParticle);
		if (!p3->pinned)
			p3->applyForce(fAeroParticle);
	}
}

Triangle::~Triangle()
{
}

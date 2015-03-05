#include "SpringDamper.h"


SpringDamper::SpringDamper()
{
	length = 1.0; 
	springConstant = 1.0; 
	dampingFactor = 1.0; 
	restLength = 1.0; 
}

SpringDamper::SpringDamper(Particle *p1, Particle *p2)
{
	this->p1 = p1; 
	this->p2 = p2; 
	length = 0.1;
	springConstant = 10;
	dampingFactor = 0.5;
	restLength = 0.1;
}

//spring dampers: up down, cross 
void SpringDamper::computeForce()
{
	//length is vec->dist
	Vector3 *vec = new Vector3(p1->position - p2->position);	
	//unit length vec from p1 to p2
	Vector3 *e = new Vector3((p1->position - p2->position) / vec->Mag()); //or vec->Normalize(); 
	
	//dampening force
	Vector3 *velocity = new Vector3((p1->velocity * (*e)) - (p2->velocity * (*e)));
	Vector3 dForce = -dampingFactor*(*velocity); 

	float x = vec->Mag2() - length; 

	Vector3 X = x*(*e); 

	Vector3 springForce = -springConstant * X; 
	
	//not right?
	Vector3 sdForce = springForce - dForce; 
	if (p1->mass > 0)
		p1->applyForce(sdForce*(*e)); 
	if (p2->mass > 0)
		p2->applyForce(-1 * (sdForce*(*e))); 

	//then add the forces?
}

void SpringDamper::draw()
{
	glBegin(GL_LINES);
		glVertex3f(p1->position.x, p1->position.y, p1->position.z); 
		glVertex3f(p2->position.x, p2->position.y, p2->position.z);
	glEnd();
}
SpringDamper::~SpringDamper()
{
}

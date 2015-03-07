#include "SpringDamper.h"



SpringDamper::SpringDamper(Particle *p1, Particle *p2)
{
	this->p1 = p1; 
	this->p2 = p2; 	

	//for 0.005
	//springConstant = 300;
	//dampingFactor = 36;

	//for 0.001; 
	//springConstant = 650;
	//dampingFactor = 180;

	//for 0.01; 
	springConstant = 640;
	dampingFactor = 13;
	Vector3 rest = this->p1->position - this->p2->position; 
	restLength = rest.Mag(); 
	restLength2 = rest.Mag2(); 
}

//spring dampers: up down, cross 
void SpringDamper::computeForce()
{
	Vector3 pulledDist = Vector3(p1->position - p2->position);
	float dist = pulledDist.Mag();
	//Vector3 e = pulledDist / pulledDist.Mag(); 
	Vector3 e = pulledDist / dist; //pulledDist.Normalize(); 
	float v1 = e.Dot(p1->velocity); 
	float v2 = e.Dot(p2->velocity);

	/*if (dist > restLength)
		std::cout << "stuff" << std::endl;*/

	float sForce = -springConstant*(dist- restLength);
	//Vector3 dForce = -dampingFactor*(v1 - v2);
	float dForce = -dampingFactor*(v1 - v2); 
	//float sdForce = -springConstant*(dist - restLength) - dampingFactor*(v1 - v2);
	float sdForce = sForce + dForce; 


	//need to find a way not to apply forces... 
	if (!p1->pinned)
		p1->applyForce(sdForce*e);	
	if (!p2->pinned) 
		p2->applyForce(-1 * sdForce*e);			
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

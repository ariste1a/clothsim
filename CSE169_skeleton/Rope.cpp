#include "Rope.h"


Rope::Rope()
{
}

Rope::Rope(Particle *p1, Particle *p2, int links)
{
	this->p1 = p1; 
	this->p2 = p2; 
	this->length;
	this->links = links; 

	Vector3 dir = p2->position - p1->position; 
	Vector3 link = dir / links; 
	this->length = dir.Mag();
	Vector3 pos = p1->position; 
	float iterations = length / link.Mag(); 
	rope.push_back(p1);
	for (int i = 1; i <iterations ; i++)
	{
		pos += Vector3(link); 
		rope.push_back(new Particle(pos.x, pos.y, pos.z));		
		SpringDamper *damper = new SpringDamper(rope[i - 1], rope[i]);
		damper->restLength = link.Mag(); 
		dampers.push_back(damper); 
		damper->springConstant = 1000;
		damper->dampingFactor = 23;		
	}
	dampers.push_back(new SpringDamper(rope.back(), p2)); 	
	dir.Normalize(); 
}

void Rope::draw()
{
	glBegin(GL_LINE);
	for (int i = 0; i < dampers.size(); i++)
	{
		dampers[i]->draw(); 
	}
}


void Rope::reset()
{
	Vector3 dir = p2->position - p1->position;
	Vector3 link = dir / links;
	this->length = dir.Mag();
	Vector3 pos = p1->position;
	float iterations = length / link.Mag();
	rope.push_back(p1);
	for (int i = 1; i < iterations; i++)
	{
		pos += Vector3(link);
		rope.at(i)->position = Vector3(pos.x, pos.y, pos.z);			
	}
	dir.Normalize();
}
Rope::~Rope()
{
}

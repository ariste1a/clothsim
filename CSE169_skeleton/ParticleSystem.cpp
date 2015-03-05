#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{	

	
	//remake this to have a set length, but increase particle density instead. 
	float length = 10; 
	//make triangles diagonally
	for (int i = 0; i < numParticles; i++)
	{		
		for (int j = 0; j < numParticles; j++)
		{
			//need to make top row with weight 0 
			Particle *particle = new Particle(); 
			//positioning them? then need to make top row solid and immutable 
			particle->position = Vector3(i / 10.0, j / 10.0, 0);
			this->clothParticles[i][j] = particle;
			this->particles.push_back(particle);				
		}
		
	}



	//add the cross spirng constraints 
	for (int i = 0; i < numParticles; i++)
	{
		for (int j = 0; j < numParticles; j++)
		{
			SpringDamper *sd; 
			if (i > 0)
			{
				sd = new SpringDamper(this->clothParticles[i - 1][j], this->clothParticles[i][j]); 
				this->springDampers.push_back(sd); 
			}

			if (j >  0)
			{
				sd = new SpringDamper(this->clothParticles[i][j], this->clothParticles[i][j-1]);
				this->springDampers.push_back(sd);
			}

		}

	}
	
	this->clothParticles[0][0]->mass = 0; 
	this->clothParticles[(numParticles / 2)][0]->mass = 0;
	this->clothParticles[numParticles - 1][0]->mass = 0;
 

}

void ParticleSystem::update(float deltaTime) {
	// Compute forces
	Vector3 gravity(0, -9.8, 0);	
	//need to add up all the forces at every single step. first step is gravity, then the next are the spring forces, then the spring dampen forces. 

	/*
	for (int i = 0; i < numParticles; i++)
	{
		for (int j = 0; j < numParticles; j++)
		{
			Vector3 force = gravity*clothParticles[i][j]->mass; // f=mg
			clothParticles[i][j]->applyForce(force);			
		}

	}*/

	//use this instead
	for (int i = 0; i < particles.size(); i++)
	{
		//particles[i]->draw(); 
		Vector3 force = gravity*particles[i]->mass; // f=mg
		particles[i]->applyForce(force);
	}
	


	for (int i = 0; i < springDampers.size(); i++)
	{
		springDampers[i]->computeForce(); 
	}
	
	/*
	- For each particle: Apply gravity
	- For each spring-damper: Compute & apply forces
	- For each triangle: Compute & apply aerodynamic forces
	*/

	// Integrate
	for (int i = 0; i < particles.size(); i++)
	{
		//springDampers[i]->computeForce();
		this->particles[i]->update(deltaTime); 
	}
}


//make triangles diagonally
void ParticleSystem::draw()
{
	glBegin(GL_POINTS);
		for (int i = 0; i < particles.size(); i++)
		{
			particles[i]->draw(); 
		}
	glEnd();
	
	GLfloat cyan[] = { 0.f, .8f, .8f, 1.f };
	GLfloat white[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat shiny[] = { 50.f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_SHININESS, shiny);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < numParticles-1; i++)
	{
		for (int j = 0; j < numParticles-1; j++)
		{
			glColor3f(0.0, 1, 0.0);
			Vector3 p1 = clothParticles[i][j]->position;
			Vector3 p2 = clothParticles[i+1][j]->position; 			
			Vector3 p3 = clothParticles[i][j+1]->position;
			
			glVertex3f(p1.x, p1.y, p1.z);
			glVertex3f(p2.x, p2.y, p2.z);
			glVertex3f(p3.x, p3.y, p3.z);

			glColor3f(0.0, 0.0, 1.0);

			Vector3 p12 = clothParticles[i][j+1]->position;
			Vector3 p22 = clothParticles[i+1][j+1]->position;
			Vector3 p32 = clothParticles[i+1][j]->position;

			glVertex3f(p32.x, p32.y, p32.z);
			glVertex3f(p12.x, p12.y, p12.z);
			glVertex3f(p22.x, p22.y, p22.z);								
		}
	}
	glEnd(); 
	/*
	glBegin(GL_QUADS);
	for (int i = 0; i < numParticles - 1; i++)
	{
		for (int j = 0; j < numParticles - 1; j++)
		{
			glColor3f(0.0, 1, 0.0);
			Vector3 p1 = clothParticles[i][j]->position;
			Vector3 p2 = clothParticles[i + 1][j]->position;
			Vector3 p3 = clothParticles[i][j + 1]->position;
			Vector3 p4 = clothParticles[i + 1][j + 1]->position; 

			glVertex3f(p4.x, p4.y, p4.z);
			glVertex3f(p3.x, p3.y, p3.z);
			glVertex3f(p1.x, p1.y, p1.z);
			glVertex3f(p2.x, p2.y, p2.z);
		}
	}
	glEnd(); */ 
	/*
	for (int i = 0; i < springDampers.size(); i++)
	{
		springDampers[i]->draw(); 
	}*/ 

}

void ParticleSystem::reset()
{
	for (int i = 0; i < numParticles; i++)
	{
		for (int j = 0; j < numParticles; j++)
		{					
			this->clothParticles[i][j]->position.x = i / 10.0; 
			this->clothParticles[i][j]->position.y = j / 10.0;
			this->clothParticles[i][j]->position.z = 0; 

			this->clothParticles[i][j]->force.Zero(); 
			this->clothParticles[i][j]->velocity.Zero(); 			
		}

	}
}
ParticleSystem::~ParticleSystem()
{
}
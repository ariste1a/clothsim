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
			particle->position = Vector3(i , j , 0);
			this->clothParticles[i][j] = particle;
			this->particles.push_back(particle);
			/*if (j == numParticles - 1)
				particle->pinned = true;*/
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

			if (i > 0 && j > 0)
			{
				sd = new SpringDamper(this->clothParticles[i][j], this->clothParticles[i-1][j - 1]);
				this->springDampers.push_back(sd);
				sd = new SpringDamper(this->clothParticles[i][j - 1], this->clothParticles[i - 1][j]);
				this->springDampers.push_back(sd);
			}
		}
	}
	
	
	//triangle surfaces
	for (int i = 0; i < numParticles-1; i++)
	{
		for (int j = 0; j < numParticles-1; j++)
		{												
			Triangle *tri1 = new Triangle(clothParticles[i][j], clothParticles[i + 1][j], clothParticles[i][j + 1]);
			Triangle *tri2 = new Triangle(clothParticles[i][j + 1], clothParticles[i + 1][j + 1], clothParticles[i + 1][j]);
			triangles.push_back(tri1);
			triangles.push_back(tri2);
		}
	}
	this->clothParticles[0][numParticles - 1]->pinned = true; 
	this->clothParticles[(numParticles / 2)][numParticles -1]->pinned = true;
	this->clothParticles[numParticles - 1][numParticles - 1]->pinned = true; 

}

void ParticleSystem::update(float deltaTime, Vector3 &wind) {
	// Compute forces

	Vector3 gravity(0, -9.8, 0);	
	for (int i = 0; i < particles.size(); i++)
	{
		//particles[i]->draw(); 
		if (!particles[i]->pinned)
		{
			Vector3 force = gravity*particles[i]->mass; // f=mg
			particles[i]->applyForce(force);
		}
	}


	for (int i = 0; i < springDampers.size(); i++)
	{
		springDampers[i]->computeForce();
	}

	for (int i = 0; i < triangles.size(); i++)
	{
		triangles[i]->computeForces(wind);
	}

	for (int i = 0; i < particles.size(); i++)
	{	
		this->particles[i]->update(deltaTime);
	}

}


//make triangles diagonally
void ParticleSystem::draw()
{
	//glBegin(GL_POINTS);
	//	for (int i = 0; i < particles.size(); i++)
	//	{
	//		particles[i]->draw(); 
	//	}
	//glEnd();


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
			//glColor3f(0.0, 1, 0.0);
			Vector3 p1 = clothParticles[i][j]->position;
			Vector3 p2 = clothParticles[i+1][j]->position; 			
			Vector3 p3 = clothParticles[i][j+1]->position;
			
			glVertex3f(p1.x, p1.y, p1.z);
			glVertex3f(p2.x, p2.y, p2.z);
			glVertex3f(p3.x, p3.y, p3.z);

			//glColor3f(0.0, 0.0, 1.0);

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
	} */

}

void ParticleSystem::reset()
{
	for (int i = 0; i < numParticles; i++)
	{
		for (int j = 0; j < numParticles; j++)
		{					
			this->clothParticles[i][j]->position.x = i; 
			this->clothParticles[i][j]->position.y = j;
			this->clothParticles[i][j]->position.z = 0; 

			this->clothParticles[i][j]->force.Zero(); 
			this->clothParticles[i][j]->velocity.Zero(); 			
		}

	}
}

void ParticleSystem::move(Matrix34 pos)
{
	pos.Transform(clothParticles[0][numParticles - 1]->position, clothParticles[0][numParticles - 1]->position);
	pos.Transform(clothParticles[(numParticles / 2)][numParticles - 1]->position, clothParticles[(numParticles / 2)][numParticles - 1]->position);
	pos.Transform(clothParticles[numParticles - 1][numParticles - 1]->position, clothParticles[numParticles - 1][numParticles - 1]->position);
}

void ParticleSystem::rotate(Matrix34 matrix)
{
	//for (int i = 0; i < numParticles; i++)
	//{
	//	clothParticles[i][numParticles - 1]->position;
	//	matrix.Transform(clothParticles[i][numParticles - 1]->position, clothParticles[i][numParticles - 1]->position);
	//}
	Vector3 origin; 
	Vector3 neg = -clothParticles[(numParticles / 2)][numParticles - 1]->position;
	Matrix34 negate; 
	Matrix34 rot; 
	Matrix34 normal;
	Matrix34 final;
	
	negate.MakeTranslate(neg); 
	rot = matrix; 
	normal.MakeTranslate(clothParticles[(numParticles / 2)][numParticles - 1]->position);

	/*final.Dot(negate, rot);
	final.Dot(final, normal);*/

	final.Dot(rot, negate);
	final.Dot(normal, final);

	final.Transform(clothParticles[0][numParticles - 1]->position, clothParticles[0][numParticles - 1]->position);
	//matrix.Transform(clothParticles[(numParticles / 2)][numParticles - 1]->position, clothParticles[(numParticles / 2)][numParticles - 1]->position);
	final.Transform(clothParticles[numParticles - 1][numParticles - 1]->position, clothParticles[numParticles - 1][numParticles - 1]->position);
}

ParticleSystem::~ParticleSystem()
{
}
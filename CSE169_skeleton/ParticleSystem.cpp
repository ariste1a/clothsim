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

			Vector3 p0 = clothParticles[i][j]->position;
			Vector3 p1 = clothParticles[i + 1][j]->position;
			Vector3 p2 = clothParticles[i][j + 1]->position;
			Vector3 p3 = clothParticles[i + 1][j + 1]->position;

			Triangle *tri1 = new Triangle(clothParticles[i][j], clothParticles[i + 1][j], clothParticles[i + 1][j + 1]);
			Triangle *tri2 = new Triangle(clothParticles[i][j], clothParticles[i + 1][j + 1], clothParticles[i][j + 1]);
			//Triangle *tri1 = new Triangle(clothParticles[i][j], clothParticles[i + 1][j], clothParticles[i][j + 1]);
			//Triangle *tri2 = new Triangle(clothParticles[i][j + 1], clothParticles[i + 1][j + 1], clothParticles[i + 1][j]);
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

	for (int i = 0; i < springDampers.size(); i++)
	{
		Vector3 pulledDist = Vector3(springDampers[i]->p1->position - springDampers[i]->p2->position);
		float dist = pulledDist.Mag();
		float restLength = springDampers[i]->restLength;
		Vector3 e = pulledDist / dist; //pulledDist.Normalize(); 

		/*if (dist >(restLength*0.05) && !springDampers[i]->p1->pinned && springDampers[i]->p1->position.y > -10)
		{
			Vector3 moveBack = -e*0.01;
			springDampers[i]->p1->position += moveBack;
		}

		if (dist > (restLength*0.1) && !springDampers[i]->p2->pinned && springDampers[i]->p2->position.y > -10)
		{
			Vector3 moveBack = e*0.01;
			springDampers[i]->p2->position += moveBack;
		}*/
	}

}

/*
AFTER updating
for each node
zero the normals

for each triangle
compute normal
for each node tri vert(3)
add normal

for each node normalize
*/

//make triangles diagonally
void ParticleSystem::draw()
{
	//glBegin(GL_POINTS);
	//	for (int i = 0; i < particles.size(); i++)
	//	{
	//		particles[i]->draw(); 
	//	}
	//glEnd();
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->normal.Zero(); 
	}
	for (int i = 0; i < triangles.size(); i++)
	{
		//triangles[i]->computeNormal();
		Vector3 norm = *new Vector3();
		norm.Cross(triangles[i]->p2->position - triangles[i]->p1->position, triangles[i]->p3->position - triangles[i]->p1->position);
		triangles[i]->p1->normal += norm;
		triangles[i]->p2->normal += norm;
		triangles[i]->p3->normal += norm;
	}
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->normal.Normalize();
	}

	GLfloat cyan[] = { 0.f, .5f, .5f, 1.f };
	GLfloat white[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat shiny[] = { 50.f };

	glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_SHININESS, shiny);

	GLfloat red[] = { 0.8f, 0.0f, .8f, 1.f };
	GLfloat white2[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat shiny2[] = { 1.0f };

	glMaterialfv(GL_BACK, GL_DIFFUSE, red);
	glMaterialfv(GL_BACK, GL_SPECULAR, white2);
	glMaterialfv(GL_BACK, GL_SHININESS, shiny2);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < numParticles-1; i++)
	{
		for (int j = 0; j < numParticles-1; j++)
		{
			//glColor3f(0.0, 1, 0.0);
			Vector3 p0 = clothParticles[i][j]->position;
			Vector3 p1 = clothParticles[i + 1][j]->position;
			Vector3 p2 = clothParticles[i][j + 1]->position;
			Vector3 p3 = clothParticles[i + 1][j + 1]->position;			

			Vector3 p0n = clothParticles[i][j]->normal;
			Vector3 p1n = clothParticles[i + 1][j]->normal;
			Vector3 p2n = clothParticles[i][j + 1]->normal;
			Vector3 p3n = clothParticles[i + 1][j + 1]->normal;			

			glNormal3f(p0n.x, p0n.y, p0n.z); 			
			glVertex3f(p0.x, p0.y, p0.z);
			glNormal3f(p1n.x, p1n.y, p1n.z);
			glVertex3f(p1.x, p1.y, p1.z);
			glNormal3f(p3n.x, p3n.y, p3n.z);
			glVertex3f(p3.x, p3.y, p3.z);
			
			glNormal3f(p0n.x, p0n.y, p0n.z);
			glVertex3f(p0.x, p0.y, p0.z);
			glNormal3f(p3n.x, p3n.y, p3n.z);
			glVertex3f(p3.x, p3.y, p3.z);
			glNormal3f(p2n.x, p2n.y, p2n.z);
			glVertex3f(p2.x, p2.y, p2.z);								
		}
	}
	glEnd(); 

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
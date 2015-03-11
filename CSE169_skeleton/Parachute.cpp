#include "Parachute.h"


Parachute::Parachute()
{
	//parachute
	float x; 
	float y; 
	float z; 
	for (int i = 0; i < numParticles; i++)
	{
		for (int j = 0; j < numParticles; j++)
		{
			Particle *particle = new Particle();
			particle->position = Vector3(i, 100, j);
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
				sd = new SpringDamper(this->clothParticles[i][j], this->clothParticles[i][j - 1]);
				this->springDampers.push_back(sd);
			}

			if (i > 0 && j > 0)
			{
				sd = new SpringDamper(this->clothParticles[i][j], this->clothParticles[i - 1][j - 1]);
				this->springDampers.push_back(sd);
				sd = new SpringDamper(this->clothParticles[i][j - 1], this->clothParticles[i - 1][j]);
				this->springDampers.push_back(sd);
			}
		}
	}


	//triangle surfaces
	for (int i = 0; i < numParticles - 1; i++)
	{
		for (int j = 0; j < numParticles - 1; j++)
		{
			Triangle *tri1 = new Triangle(clothParticles[i][j], clothParticles[i + 1][j], clothParticles[i][j + 1]);
			Triangle *tri2 = new Triangle(clothParticles[i][j + 1], clothParticles[i + 1][j + 1], clothParticles[i + 1][j]);
			triangles.push_back(tri1);
			triangles.push_back(tri2);
		}
	}		


	//box position and offset
	x = 8;
	y = 90;
	z = 8;
	float offset = 4.0;

	box[0][0][0] = new Particle(x, y, z);	
	particles.push_back(box[0][0][0]);
	box[1][0][0] = new Particle(x+offset, y, z);
	particles.push_back(box[1][0][0]);
	box[0][1][0] = new Particle(x, y+offset, z);
	particles.push_back(box[0][1][0]);
	box[1][1][0] = new Particle(x+offset, y + offset, z);
	particles.push_back(box[1][1][0]);

	box[0][0][1] = new Particle(x, y, z+offset);
	particles.push_back(box[0][0][1]);
	box[1][0][1] = new Particle(x + offset, y, z+offset);
	particles.push_back(box[1][0][1]);
	box[0][1][1] = new Particle(x, y + offset, z+offset);
	particles.push_back(box[0][1][1]);
	box[1][1][1] = new Particle(x + offset, y + offset, z+offset);
	particles.push_back(box[1][1][1]);

	SpringDamper *sd;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				if(i > 0)
				{
					sd = new SpringDamper(box[i][j][k], box[i-i][j][k]); 
					springDampers.push_back(sd);
				}
				if(j > 0)
				{
					sd = new SpringDamper(box[i][j][k], box[i][j-1][k]); 
					springDampers.push_back(sd);
				}
				if(k > 0)
				{
					sd = new SpringDamper(box[i][j][k], box[i][j][k-1]); 
					springDampers.push_back(sd);
				}
				if (i > 0 && j > 0)
				{
					sd = new SpringDamper(box[i][j][k], box[i-1][j-1][k]); 
					springDampers.push_back(sd);
					sd = new SpringDamper(box[i][j-1][k], box[i-1][j][k]);
					springDampers.push_back(sd);
				}
				if(i> 0 && j > 0 && k > 0)
				{
					sd = new SpringDamper(box[i][j][k], box[i - 1][j - 1][k-1]);
					springDampers.push_back(sd);
					sd = new SpringDamper(box[i][j - 1][k], box[i - 1][j][k-1]);
					springDampers.push_back(sd);
				}
				if (j > 0 && k > 0)
				{
					sd = new SpringDamper(box[i][j][k], box[i][j - 1][k - 1]);
					springDampers.push_back(sd);
					sd = new SpringDamper(box[i][j - 1][k], box[i][j][k - 1]);
					springDampers.push_back(sd);
				}
				if (i > 0 && k > 0)
				{
					sd = new SpringDamper(box[i][j][k], box[i-1][j][k - 1]);
					springDampers.push_back(sd);
					sd = new SpringDamper(box[i-1][j][k], box[i][j][k - 1]);
					springDampers.push_back(sd);
				}
			}
		}
	}

	Rope *one = new Rope(box[0][1][0], clothParticles[0][0], 30); 
	Rope *two = new Rope(box[1][1][0], clothParticles[numParticles-1][0], 30);
	Rope *three = new Rope(box[0][1][1], clothParticles[0][numParticles - 1], 30);
	Rope *four = new Rope(box[1][1][1], clothParticles[numParticles - 1][numParticles - 1], 30);

	ropes.push_back(one); 
	ropes.push_back(two);
	ropes.push_back(three);
	ropes.push_back(four);

	springDampers.insert(springDampers.end(), one->dampers.begin(), one->dampers.end()); 
	springDampers.insert(springDampers.end(), two->dampers.begin(), two->dampers.end());
	springDampers.insert(springDampers.end(), three->dampers.begin(), three->dampers.end());
	springDampers.insert(springDampers.end(), four->dampers.begin(), four->dampers.end());

	particles.insert(particles.end(), one->rope.begin(), one->rope.end());
	particles.insert(particles.end(), two->rope.begin(), two->rope.end());
	particles.insert(particles.end(), three->rope.begin(), three->rope.end());
	particles.insert(particles.end(), four->rope.begin(), four->rope.end());

}

void Parachute::draw()
{
	//glBegin(GL_POINTS);
	//	for (int i = 0; i < particles.size(); i++)
	//	{
	//		particles[i]->draw(); 
	//	}
	//glEnd();
	glDisable(GL_CULL_FACE);	
	GLfloat cyan[] = { 0.5f, .5f, 0, 1.f };
	GLfloat white[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat shiny[] = { 50.f };

	glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_SHININESS, shiny);


	for (int i = 0; i < springDampers.size(); i++)
	{
		springDampers[i]->draw(); 
	}
	
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < numParticles - 1; i++)
	{
		for (int j = 0; j < numParticles - 1; j++)
		{
			//glColor3f(0.0, 1, 0.0);
			Vector3 p0 = clothParticles[i][j]->position;
			Vector3 p1 = clothParticles[i + 1][j]->position;
			Vector3 p3 = clothParticles[i + 1][j + 1]->position;
			Vector3 p2 = clothParticles[i][j + 1]->position;

			//glNormal3f(normal.x, normal.y, normal.z); 			
			glVertex3f(p0.x, p0.y, p0.z);
			glVertex3f(p1.x, p1.y, p1.z);
			glVertex3f(p3.x, p3.y, p3.z);

			//glColor3f(0.0, 0.0, 1.0);

			//normal.Zero(); 
			//glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(p0.x, p0.y, p0.z);
			glVertex3f(p3.x, p3.y, p3.z);
			glVertex3f(p2.x, p2.y, p2.z);
		}
	}
	glEnd();

	glEnable(GL_CULL_FACE);	

}

void Parachute::update(float deltaTime, Vector3 &wind)
{
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
		Vector3 e = pulledDist / dist;
	}
}


void Parachute::reset()
{
	for (int i = 0; i < numParticles; i++)
	{
		for (int j = 0; j < numParticles; j++)
		{
			this->clothParticles[i][j]->position.x = i;
			this->clothParticles[i][j]->position.y = 100;
			this->clothParticles[i][j]->position.z = j;
			this->clothParticles[i][j]->force.Zero();
			this->clothParticles[i][j]->velocity.Zero();
		}
	}

	float x = 8;
	float y = 90;
	float z = 8;
	float offset = 4.0;

	box[0][0][0]->position = Vector3(x, y, z);	
	box[1][0][0]->position = Vector3(x + offset, y, z);
	box[0][1][0]->position = Vector3(x, y + offset, z);
	box[1][1][0]->position = Vector3(x + offset, y + offset, z);

	box[0][0][1]->position = Vector3(x, y, z + offset);
	box[1][0][1]->position = Vector3(x + offset, y, z + offset);
	box[0][1][1]->position = Vector3(x, y + offset, z + offset);
	box[1][1][1]->position = Vector3(x + offset, y + offset, z + offset);

	for (int i = 0; i< 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				box[i][j][k]->velocity.Zero(); 
				box[i][j][k]->force.Zero(); 
			}
		}
	}

	for (int i = 0; i < ropes.size(); i++)
	{
		ropes[i]->reset(); 
	}

}
Parachute::~Parachute()
{

}

#include "Fluid.h"
#include <cmath>

Fluid::Fluid()
{
	numParticles = 1000;
	radius = 0.05f;

	genSphere = new Sphere(radius);	
	boundCube = new Cube();
	supportRadius = 4.0f * radius;
	initParticles();
	cout << " What the heck!!!! " << supportRadius << endl;
	
}
void Fluid::update(float deltaT)
{	
	zeroOutForces();
	clearNeighbors();
	updateNeighbors();
	updateLocalDensities();
	updateParticleForces();
	updateParticles(deltaT);
}
void Fluid::draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection)
{	
	/* First draw the cubes */
	boundCube->draw(shaderProgram, view, projection);
	/* Then draw the particles */
	for (int i = 0; i < numParticles; i++)
	{
		particles[i]->draw(shaderProgram, view, projection);
	}
}
void Fluid::zeroOutForces()
{
	for (int i = 0; i < numParticles; i++)
	{
		particles[i]->zeroOutForce();		
	}
}
void Fluid::clearNeighbors()
{
	for (int i = 0; i < numParticles; i++)
	{
		particles[i]->neighbors.clear();
	}
}
void Fluid::updateNeighbors()
{
	for (int i = 0; i < numParticles; i++)
	{
		for (int j = i + 1; j < numParticles; j++)
		{
			float dist = getMag(particles[i]->position - particles[j]->position);
			if (dist <= supportRadius)
			{
				particles[i]->neighbors.push_back(particles[j]);
				particles[j]->neighbors.push_back(particles[i]);
			}
		}
	}
}
void Fluid::updateLocalDensities()
{
	for (int i = 0; i < numParticles; i++)
	{
		particles[i]->updateLocalDensity();
	}
}
void Fluid::updateParticleForces()
{
	for (int i = 0; i < numParticles; i++)
	{
		particles[i]->updateForces();
	}
}
void Fluid::updateParticles(float deltaT)
{
	for (int i = 0; i < numParticles; i++)
	{
		particles[i]->update(deltaT);
	}
}
void Fluid::initParticles()
{	
	int index = 0;
	float gridSize = cbrtf(numParticles);
	float diff = 4 * radius;
	float y = 1.0f, z = -1.0f;
	for (int i = 0; i < gridSize; i++)
	{
		float z = -1.0f;
		for (int j = 0; j < gridSize; j++)
		{
			float x = -1.0f;
			for (int k = 0; k < gridSize; k++)
			{
				WaterParticle * newPart = new WaterParticle(glm::vec3(x, y, z), genSphere, supportRadius, radius);			
				newPart->index = index++;
				particles.push_back(newPart);
				x += diff;
			}
			z += diff;
		}
		y -= diff;
	}
	particles[0]->debug = true;
}
float Fluid::getMag(glm::vec3 val)
{
	return sqrt(pow(val.x, 2) + pow(val.y, 2) + pow(val.z, 2));
}
Fluid::~Fluid()
{
}

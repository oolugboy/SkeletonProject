#include "Fluid.h"
#include <cmath>

Fluid::Fluid()
{
	numParticles = 16;
	radius = 0.1f;

	genSphere = new Sphere(radius);	
	initParticles();
	
	supportRadius = 4 * radius;
}
void Fluid::update(float deltaT)
{	
	zeroOutForces();
	clearNeighbors();
	updateNeighbors();
	updateLocalDensities();
	updateParticles(deltaT);
}
void Fluid::draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection)
{	
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
		particles[i]->visited = false;
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
	float gridSize = sqrt(numParticles);
	float diff = 4 * radius;
	float y = 2.0f, z = -1.0f;
	for (int i = 0; i < gridSize; i++)
	{
		float x = -1.0f;
		for (int j = 0; j < gridSize; j++)
		{
			WaterParticle * newPart = new WaterParticle(glm::vec3(x, y, z), genSphere, supportRadius);
			newPart->index = index++;
			particles.push_back(newPart);
			x += diff;
		}
		z += diff;
	}
}
float Fluid::getMag(glm::vec3 val)
{
	return sqrt(pow(val.x, 2) + pow(val.y, 2) + pow(val.z, 2));
}
Fluid::~Fluid()
{
}

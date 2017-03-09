#include "Cloth.h"
#include <time.h>
#include <iostream>


Cloth::Cloth(int width, float springFactor, float dampingFactor)
{
	this->width = width;	
	this->dampingFactor = springFactor;
	this->dampingFactor = dampingFactor;
	this->div = 0.1f;
	initParticles();
	vertTriMap.resize(particles.size());
	initSpringDampers();
	initTriangles();
	this->gravity = glm::vec3(0.0f, -10.0f, 0.0f);
	this->windVelocity = glm::vec3(0.0f, 0.0f, -30.0f);
}

void Cloth::initParticles()
{
	int gridSize = ((float)width / div) + 1;
	float currZ = width * -1.0f;
	//float currY = width + 1; // Just add a little space 
	float currY = 0.5f;
	for (int i = 0; i < gridSize; i++)
	{
		float currX = width * -1.0f;
		for (int j = 0; j < gridSize; j++)
		{
			Particle * newPart = new ClothParticle(glm::vec3(currX, currY, currZ));
			if (i == 0)
				newPart->fixed = true;
			particles.push_back(newPart);
			currX += div;
		}
		currZ += div;
	}
	/* Fix the corners of the cloth  */
	particles[1]->debug = true;
	//particles[2]->debug = true;
}
void Cloth::moveFixedParticles(glm::vec3 diff)
{
	int gridSize = ((float)width / div) + 1;
	for (int i = 0; i < gridSize; i++)
	{
		particles[i]->position = particles[i]->position + diff;
	}
}
void Cloth::draw(GLint shaderProgram, glm::mat4 view, glm::mat4 projection)
{
	int size = triangles.size();
	for (int i = 0; i < size; i++)
	{
		triangles[i]->draw(shaderProgram, view, projection);
	}
}
void Cloth::initTriangles()
{	
	int gridSize = ((float)width / div) + 1;
	for (int i = 1; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize - 1; j++)
		{
			int indexA = (gridSize * i) + j, indexB = (gridSize * (i - 1)) + (j + 1);
			int indexC = (gridSize * (i - 1)) + j, indexD = (gridSize * (i)) + (j + 1);
			Triangle * upperTriangle = new Triangle(particles[indexA], particles[indexB], particles[indexC]);	
			triangles.push_back(upperTriangle);

			/* Update the map */
			vertTriMap[indexA].insert(upperTriangle);
			vertTriMap[indexB].insert(upperTriangle);
			vertTriMap[indexC].insert(upperTriangle);

			Triangle * lowerTriangle = new Triangle(particles[indexA], particles[indexD], particles[indexB]);
			triangles.push_back(lowerTriangle);

			/* Update the map again */
			vertTriMap[indexA].insert(lowerTriangle);
			vertTriMap[indexD].insert(lowerTriangle);
			vertTriMap[indexB].insert(lowerTriangle);
		}
	}
}
void Cloth::initSpringDampers()
{
	int gridSize = ((float)width / div) + 1;
	float sideLength = div;
	float diagLength = sqrt(pow(sideLength, 2) + pow(sideLength, 2));
	for (int i = 1; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize - 1; j++)
		{
			int indexA = (gridSize * i) + j, indexB = (gridSize * (i - 1)) + (j + 1);
			int indexC = (gridSize * (i - 1)) + j, indexD = (gridSize * (i)) + (j + 1);

			SpringDamper * leftVert = new SpringDamper(particles[indexA], particles[indexC], sideLength);
			springDampers.push_back(leftVert);
			SpringDamper * topHoriz = new SpringDamper(particles[indexC], particles[indexB], sideLength);
			springDampers.push_back(topHoriz);
			SpringDamper * leftDiag = new SpringDamper(particles[indexC], particles[indexD], diagLength);
			springDampers.push_back(leftDiag);
			SpringDamper * rightDiag = new SpringDamper(particles[indexA], particles[indexB], diagLength);
			springDampers.push_back(rightDiag);

			if (i == (gridSize - 1))
				springDampers.push_back(new SpringDamper(particles[indexA], particles[indexD], sideLength));
			if ((j + 1) == (gridSize - 1))
				springDampers.push_back(new SpringDamper(particles[indexB], particles[indexD], sideLength));			
		}
	}
}
float Cloth::getMag(glm::vec3 val)
{
	return sqrt(pow(val.x, 2) + pow(val.y, 2) + pow(val.z, 2));
}
void Cloth::adjustWindSpeed(bool incr)
{
	glm::vec3 windDir = glm::normalize(windVelocity);
	float speed = getMag(windVelocity);
	if (incr)
	{
		if (speed <= (windMax - windOff))
		{
			windVelocity = (speed + windOff) * windDir;
		}
	}
	else
	{
		if (speed > windOff) 
		{
			windVelocity = (speed - windOff) * windDir;
		}
	}
}

void Cloth::update(float deltaT)
{
	/* Then apply gravity */
	zeroOutForces();
	applyGravity();
	applyAeroDynForce();
	updateSpringDampers(deltaT);
	updateParticles(deltaT);
	updateNormals(deltaT);
}
void Cloth::zeroOutForces()
{
	int size = particles.size();
	for (int i = 0; i < size; i++)
	{
		particles[i]->zeroOutForce();
	}
}
void Cloth::applyAeroDynForce()
{
	int size = triangles.size();
	for (int i = 0; i < size; i++)
	{
		triangles[i]->applyAeroDynForce(windVelocity);
	}
}
void Cloth::applyGravity()
{
	int size = particles.size();
	for (int i = 0; i < size; i++)
	{
		if (particles[i]->fixed == false)
		{		
			particles[i]->applyForce(gravity);
		}
	}
}
void Cloth::updateSpringDampers(float deltaT)
{
	int size = springDampers.size();
	for (int i = 0; i < size; i++)
	{
		springDampers[i]->update(deltaT);
	}
}
void Cloth::updateParticles(float deltaT)
{
	int size = particles.size();
	for (int i = 0; i < size; i++)
	{
		particles[i]->update(deltaT);
	}
}
void Cloth::updateNormals(float deltaT)
{
	int partSize = particles.size();
	int triSize = triangles.size();
	
	/* First get the normal for the triangles */
	for (int i = 0; i < triSize; i++)
	{
		triangles[i]->calcNormal();	
	} 

	/* Then get the normal for the vertices */
	for (int i = 0; i < partSize; i++)
	{
		unordered_set< Triangle *>::iterator sit = vertTriMap[i].begin();
		unordered_set< Triangle *>::iterator sen = vertTriMap[i].end();

		glm::vec3 resNorm = glm::vec3(0.0f, 0.0f, 0.0f);

		for (; sit != sen; sit++)
		{
			resNorm = resNorm + (*sit)->normal;
		}
		particles[i]->normal = glm::normalize(resNorm);
	}
	/* Need to update the triangle for rendering */
	for (int i = 0; i < triSize; i++)
	{
		triangles[i]->update(deltaT);
	}
}
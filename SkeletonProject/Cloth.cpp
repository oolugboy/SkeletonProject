#include "Cloth.h"
#include <iostream>


Cloth::Cloth(int width, float springFactor, float dampingFactor)
{
	this->width = width;	
	this->dampingFactor = springFactor;
	this->dampingFactor = dampingFactor;
	initParticles();
	vertTriMap.resize(particles.size());
	initTriangles();
}

void Cloth::initParticles()
{
	float div = 0.1f;
	int gridSize = ((float)width / div) + 1;
	float currZ = width * -1.0f;
	//float currY = width + 1; // Just add a little space 
	float currY = 0.5f;
	for (int i = 0; i < gridSize; i++)
	{
		float currX = width * -1.0f;
		for (int j = 0; j < gridSize; j++)
		{
			Particle * newPart = new Particle(glm::vec3(currX, currY, currZ));
			particles.push_back(newPart);
			currX += div;
		}
		currZ += div;
	}
	/* Fix the corners of the cloth  */
	particles[0]->fixed = particles[gridSize - 1]->fixed = true;
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
	float div = 1.0f;
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

void Cloth::update()
{
	updateNormals();
}

void Cloth::updateNormals()
{
	int partSize = particles.size();
	int triSize = triangles.size();

	cout << " The particles size is " << partSize << endl;
	cout << " The triangles size is " << triSize << endl;
	
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
		particles[i]->update();
	}

	/* Need to update the triangle for rendering */
	for (int i = 0; i < triSize; i++)
	{
		triangles[i]->update();
	}
}
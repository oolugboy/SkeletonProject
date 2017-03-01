#include "Particle.h"
#include <iostream>

using namespace std;

Particle::Particle(glm::vec3 initPosition)
{
	position = initPosition;
	velocity = force = glm::vec3(0, 0, 0);
	fixed = false;
	this->mass = 5.0f;
	debug = false;
}

void Particle::update(float deltaT)
{
	if (fixed == false)
	{
		/* if (debug)
			cout << " The resuling force " << force.x << " " << force.y << " " << force.z << endl; */
		glm::vec3 acc = force / mass;
		velocity = velocity + (acc * deltaT);
		position = position + (velocity * deltaT);
		
		/*if (debug)
			cout << position.x << " " << position.y << " " << position.z << endl;*/
	}
	if (position.y < 0)
	{
		position.y = 0 - position.y;			
		velocity = glm::vec3(0, 0, 0.2f * velocity.z);
	}
}
void Particle::applyForce(glm::vec3 force)
{
	this->force = this->force + force;
}
void Particle::zeroOutForce()
{
	this->force = glm::vec3(0, 0, 0);
}

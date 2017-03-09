#include "Particle.h"
#include <iostream>

using namespace std;

Particle::Particle()
{
	mass = 1.0f;
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}
void Particle::applyForce(glm::vec3 force)
{
	this->force = this->force + force;
}
void Particle::zeroOutForce()
{
	this->force = glm::vec3(0, 0, 0);
}
float Particle::getMag(glm::vec3 val)
{
	return sqrt(pow(val.x, 2) + pow(val.y, 2) + pow(val.z, 2));
}

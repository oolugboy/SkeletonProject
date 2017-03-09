#include "SpringDamper.h"
#include <iostream>
using namespace std;
SpringDamper::SpringDamper(Particle * a, Particle * b, float restLength)
{
	this->a = a;
	this->b = b;
	this->springConst = 3000.0f;
	this->dampFact = 40.0f;
	this->restLength = restLength;
}
glm::vec3 getPotVelocity(Particle * pat, float deltaT)
{
	glm::vec3 acc = pat->force / pat->mass;
	return pat->velocity + (acc * deltaT);
}
float getMag(glm::vec3 val)
{
	return sqrt(pow(val.x, 2) + pow(val.y, 2) + pow(val.z, 2));
}
void SpringDamper::update(float deltaT)
{
	/* Before*/
	/*if (a->debug || b->debug)
		cout << " The force before " << a->force.x << " " << a->force.y << " " << a->force.z << endl; */

	/* First get potential velocities */
	glm::vec3 v1 = a->velocity;
	glm::vec3 r1 = a->position;
	glm::vec3 v2 = b->velocity;
	glm::vec3 r2 = b->position;

	glm::vec3 e = glm::normalize(r2 - r1);
	float l = getMag(r2 - r1);

	/* First compute the spring force */
	float fSpring = -1.0f * springConst * (restLength - l);

	/* Then the damping force */
	float fDamp = -1.0f * dampFact * (glm::dot(e, v1) - glm::dot(e, v2));

	float fRes = fSpring + fDamp;

	glm::vec3 resForce = fRes * e;
	/*if(a->debug || b->debug)
		cout << " The spring reaction force " << resForce.x << " " << resForce.y << " " << resForce.z << endl; */
	a->applyForce(fRes * e);
	b->applyForce(fRes * e * (-1.0f));
}

float SpringDamper::getLength(glm::vec3 e,  float deltaT)
{
	glm::vec3 accA = a->force / a->mass;
	glm::vec3 newAPos = (a->velocity * deltaT) + (accA * pow(deltaT, 2));

	glm::vec3 accB = b->force / b->mass;
	glm::vec3 newBPos = (b->velocity * deltaT) + (accB * pow(deltaT, 2));

	glm::vec3 newPosVec = newBPos - newAPos;
	return glm::dot(newPosVec, e);
//	return getMag(newPosVec);
}

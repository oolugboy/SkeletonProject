#pragma once
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_set>
#include <time.h>
class Particle
{
public:
	float mass;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 force;
	bool fixed;
	Particle();
	glm::vec3 normal;
	virtual void update(float deltaT) = 0;	
	float getMag(glm::vec3 val);
	void applyForce(glm::vec3 force);
	void zeroOutForce();	
	bool debug;
};

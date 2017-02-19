#pragma once
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_set>
class Particle
{
public:
	float mass;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 force;
	bool fixed;
	Particle(glm::vec3 initPosition);
	glm::vec3 normal;
	void update();
	void draw();
	void applyForce();
};

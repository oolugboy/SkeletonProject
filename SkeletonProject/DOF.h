#pragma once
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#define MAXDEGREE -100000
class DOF
{
public:
	glm::vec3 rotMin;
	glm::vec3 rotMax;
	glm::vec3 pose;
	void clamp(glm::vec3 & pose);
	DOF();
	DOF(glm::vec3 pose);
	DOF(glm::vec3 pose, glm::vec3 rotMin, glm::vec3 rotMax);
	~DOF();
};


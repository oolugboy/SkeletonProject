#include "DOF.h"

DOF::DOF()
{
	this->pose = glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotMin = glm::vec3(-MAXDEGREE, -MAXDEGREE, -MAXDEGREE);
	this->rotMax = glm::vec3(MAXDEGREE, MAXDEGREE, MAXDEGREE);
}
DOF::DOF(glm::vec3 pose)
{
	this->pose = pose;
	this->rotMin = glm::vec3(-MAXDEGREE, -MAXDEGREE, -MAXDEGREE);
	this->rotMax = glm::vec3(MAXDEGREE, MAXDEGREE, MAXDEGREE);
}
DOF::DOF(glm::vec3 pose, glm::vec3 rotMin, glm::vec3 rotMax)
{
	this->pose = pose;
	//To clamp the pose in case 
	clamp(this->pose);
	this->rotMin = rotMin;
	this->rotMax = rotMax;
}
void DOF::clamp(glm::vec3 & pose)
{
	pose.x = fmax(pose.x, -MAXDEGREE);
	pose.x = fmin(pose.x, MAXDEGREE);
	pose.y = fmax(pose.y, -MAXDEGREE);
	pose.y = fmin(pose.y, MAXDEGREE);
	pose.z = fmax(pose.z, -MAXDEGREE);
	pose.z = fmin(pose.z, MAXDEGREE);
}
DOF::~DOF()
{
}

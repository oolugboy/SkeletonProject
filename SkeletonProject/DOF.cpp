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
	pose.x = fmax(pose.x, rotMin.x);
	pose.x = fmin(pose.x, rotMax.x);
	pose.y = fmax(pose.y, rotMin.y);
	pose.y = fmin(pose.y, rotMax.y);
	pose.z = fmax(pose.z, rotMin.z);
	pose.z = fmin(pose.z, rotMax.z); 
}
DOF::~DOF()
{
}

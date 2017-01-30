#ifndef SKELETON_HPP
#define SKELETON_HPP
#include "BallJoint.h"
class Skeleton
{
public:
	Skeleton(const char * fileName);
	~Skeleton();
	void update();
	void draw();
	static glm::mat4 getWorldMatrix(Joint * root, int id);
	void bindNextJoint();
	Joint * root;
	void adjustPos(glm::vec3 axis, bool incr);
private:
	char fileName[BUFSIZ];
	int currJointId;
	int numJoints;

	bool parseLoad();
};
#endif
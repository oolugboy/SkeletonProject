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
	void animatePose(glm::vec3 newPose, int jointId);
	void animateRootOffset(glm::vec3 newOffset);
private:
	char fileName[BUFSIZ];
	int currJointId;
	int numJoints;

	bool parseLoad();
};
#endif